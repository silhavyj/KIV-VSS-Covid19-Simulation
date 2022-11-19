#include <cmath>

#include "spdlog/spdlog.h"

#include "utils.h"
#include "singleton.h"
#include "simulation.h"

namespace kiv_vss
{
    CSimulation::CSimulation()
        : m_config{Singleton<TConfig>::Get_Instance()},
          m_statistics{},
          m_time{0},
          m_system_saturated{false},
          m_max_number_of_infected_people{0}
    {
        Generate_Population();
    }

    void CSimulation::Update()
    {
        ++m_time;
        Move_People_Around();
        Spread_Virus();
    }

    void CSimulation::Add_Popular_Location(const CLocation& location)
    {
        m_popular_locations.push_back(location);
    }

    const CSimulation::TStatistics& CSimulation::Get_Statistics() const
    {
        return m_statistics;
    }

    bool CSimulation::Is_System_Saturated() const
    {
        return m_system_saturated;
    }

    const std::vector<CPerson>& CSimulation::Get_People() const
    {
        return m_people;
    }

    bool CSimulation::Is_Simulation_Over() const
    {
        return !m_statistics.number_of_infected_people.empty() &&
               0 == m_statistics.number_of_infected_people.back();
    }

    std::size_t CSimulation::Get_Maximum_Number_Of_Infected_People() const
    {
        return m_max_number_of_infected_people;
    }

    const std::vector<CLocation>& CSimulation::Get_Popular_Locations() const
    {
        return m_popular_locations;
    }

    void CSimulation::Generate_Population()
    {
        // TODO temporary (delete)
        for (std::size_t i = 0; i < 3; ++i)
        {
            m_popular_locations.emplace_back(CLocation::Generate_Random_In_Square_Location(0.25 * m_config->general.world_size, 0.75 * m_config->general.world_size));
        }

        const auto number_of_people_in_self_isolation = static_cast<std::size_t>(
            static_cast<double>(m_config->general.number_of_people) * m_config->general.ratio_of_people_in_self_isolation
        );

        std::size_t number_of_infected_people = 0;
        m_people.reserve(m_config->general.number_of_people);

        for (std::size_t i = 0; i < m_config->general.number_of_people; ++i)
        {
            const bool self_isolating = i < number_of_people_in_self_isolation;
            number_of_infected_people += !self_isolating;
            const bool initially_infected = !self_isolating && number_of_infected_people <= m_config->general.number_of_initially_infected_people;

            m_people.emplace_back(CLocation::Generate_Random_In_Square_Location(0, m_config->general.world_size));
            m_mobility_managers.emplace_back(&m_people[i], &m_popular_locations, self_isolating);
            m_infection_managers.emplace_back(&m_people[i], &m_system_saturated, initially_infected);
        }
    }

    void CSimulation::Move_People_Around()
    {
        for (auto& manager : m_mobility_managers)
        {
            manager.Update();
        }
        for (auto& manager : m_infection_managers)
        {
            manager.Update();
        }
    }

    void CSimulation::Spread_Virus()
    {
        TStatistics_Record record{};

        for (std::size_t i = 0; i < m_config->general.number_of_people; ++i)
        {
            for (std::size_t j = i + 1; j < m_config->general.number_of_people; ++j)
            {
                auto& manager1 = m_infection_managers.at(i);
                auto& manager2 = m_infection_managers.at(j);

                if (Try_To_Infect(manager1, manager2))
                {
                    if (manager1.Get_Person()->Get_Infection_State() == CPerson::NInfection_State::Susceptible)
                    {
                        manager1.Infect();
                    }
                    else
                    {
                        manager2.Infect();
                    }
                }
            }
            Update_Statistics_Record(record, m_people.at(i), m_infection_managers.at(i).Get_Infection_Count());
        }
        if (m_time % 24 == 0)
        {
            Update_Statistics(record);
            m_max_number_of_infected_people = std::max(m_max_number_of_infected_people, static_cast<std::size_t>(record.number_of_infected_people));
            float infected_percentage = record.number_of_infected_people / m_config->general.number_of_people;
            m_system_saturated = infected_percentage > m_config->general.saturation_level;
        }
    }

    inline void CSimulation::Update_Statistics(const TStatistics_Record& record)
    {
        m_statistics.number_of_susceptible_people.emplace_back(record.number_of_susceptible_people);
        m_statistics.number_of_contractions_per_person.emplace_back(record.number_of_infections_per_person);
        m_statistics.number_of_fatalities.emplace_back(record.number_of_fatalities);
        m_statistics.number_of_immune_people.emplace_back(record.number_of_immune_people);
        m_statistics.number_of_infected_people.emplace_back(record.number_of_infected_people);

        // TODO think through
        m_statistics.time.emplace_back(m_time / 24.0);
    }

    inline void CSimulation::Update_Statistics_Record(TStatistics_Record& record, const CPerson& person, std::size_t infected_count)
    {
        record.number_of_infections_per_person += static_cast<double>(infected_count) / static_cast<double>(m_people.size());
        const auto state = person.Get_Infection_State();

        switch (state)
        {
            case CPerson::NInfection_State::Susceptible:
                ++record.number_of_susceptible_people;
                break;

            case CPerson::NInfection_State::Infected:
                ++record.number_of_infected_people;
                break;

            case CPerson::NInfection_State::Immune:
                ++record.number_of_immune_people;
                break;

            case CPerson::NInfection_State::Dead:
                ++record.number_of_fatalities;
                break;

            default:
                spdlog::error("Unknown infection state detected");
                break;
        }
    }

    bool CSimulation::Try_To_Infect(const CInfection_Model& infected_person_manager, const CInfection_Model& vulnerable_person_manager) const
    {
        const auto person1 = infected_person_manager.Get_Person();
        const auto person2 = vulnerable_person_manager.Get_Person();

        const auto person1_state = person1->Get_Infection_State();
        const auto person2_state = person2->Get_Infection_State();

        if (!((person1_state == CPerson::NInfection_State::Susceptible && person2_state == CPerson::NInfection_State::Infected) ||
              (person2_state == CPerson::NInfection_State::Susceptible && person1_state == CPerson::NInfection_State::Infected)))
        {
            return false;
        }

        if ((person1->Get_Current_Location() - person2->Get_Current_Location()) > m_config->disease.transmission_distance)
        {
            return false;
        }
        if (person1->Is_Home() ^ person2->Is_Home())
        {
            return false;
        }
        if (person1->Is_Home())
        {
            return utils::Try_Event(m_config->disease.transmission_prob_at_home);
        }

        return utils::Try_Event(m_config->disease.transmission_prob_on_move);
    }
}