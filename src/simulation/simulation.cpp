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
        // Generate the population.
        Generate_Population();
    }

    void CSimulation::Update()
    {
        // Another hour has passed by.
        ++m_time;

        // Move people around and spread the virus among people.
        Move_People_Around();
        Spread_Virus();
    }

    void CSimulation::Add_Popular_Location(const CLocation& location)
    {
        m_popular_locations.push_back(location);
    }

    const CSimulation::TStatistics& CSimulation::Get_Statistics() const noexcept
    {
        return m_statistics;
    }

    bool CSimulation::Is_System_Saturated() const noexcept
    {
        return m_system_saturated;
    }

    const std::vector<CPerson>& CSimulation::Get_People() const noexcept
    {
        return m_people;
    }

    bool CSimulation::Is_Simulation_Over() const noexcept
    {
        return !m_statistics.number_of_infected_people.empty() &&
               0 == m_statistics.number_of_infected_people.back();
    }

    std::size_t CSimulation::Get_Maximum_Number_Of_Infected_People() const noexcept
    {
        return m_max_number_of_infected_people;
    }

    const std::vector<CLocation>& CSimulation::Get_Popular_Locations() const noexcept
    {
        return m_popular_locations;
    }

    void CSimulation::Generate_Population()
    {
        // Calculate how many people will be voluntarily quarantining.
        // The user can set the percentage through the GUI.
        const auto number_of_people_in_self_isolation = static_cast<std::size_t>(
            static_cast<double>(m_config->general.number_of_people) * m_config->general.ratio_of_people_in_self_isolation
        );

        // Allocate memory for all the people.
        m_people.reserve(m_config->general.number_of_people);

        std::size_t number_of_infected_people = 0;
        for (std::size_t i = 0; i < m_config->general.number_of_people; ++i)
        {
            // First n people will be voluntarily in self-isolation.
            const bool self_isolating = i < number_of_people_in_self_isolation;

            // Increment the number of infected people (if the person is not quarantining).
            number_of_infected_people += !self_isolating;

            // Calculate whether the person will be initially infected or not.
            const bool initially_infected = !self_isolating && number_of_infected_people <= m_config->general.number_of_initially_infected_people;

            // Create a person along with its corresponding managers (mobility and infection model).
            m_people.emplace_back(CLocation::Generate_Random_In_Square_Location(0, m_config->general.world_size));
            m_mobility_managers.emplace_back(&m_people[i], &m_popular_locations, self_isolating);
            m_infection_managers.emplace_back(&m_people[i], &m_system_saturated, initially_infected);
        }
    }

    void CSimulation::Move_People_Around()
    {
        // Move people around.
        for (auto& manager : m_mobility_managers)
        {
            manager.Update();
        }

        // Update their infection state (decrement the counter).
        for (auto& manager : m_infection_managers)
        {
            manager.Update();
        }
    }

    void CSimulation::Spread_Virus()
    {
        TStatistics_Record record{};

        // Iterate over each pair of people.
        for (std::size_t i = 0; i < m_config->general.number_of_people; ++i)
        {
            for (std::size_t j = i + 1; j < m_config->general.number_of_people; ++j)
            {
                // Get the two people.
                auto& manager1 = m_infection_managers.at(i);
                auto& manager2 = m_infection_managers.at(j);

                // Check if one can infected another.
                if (Try_To_Infect(manager1, manager2))
                {
                    // Check which one was infected and infected the other person.
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

            // Update the statistical record calculated within this update of the simulation.
            Update_Statistics_Record(record, m_people.at(i), m_infection_managers.at(i).Get_Infection_Count());
        }

        // Capture the statistics every 24 hours.
        if (m_time % 24 == 0)
        {
            Update_Statistics(record);
            m_max_number_of_infected_people = std::max(m_max_number_of_infected_people, static_cast<std::size_t>(record.number_of_infected_people));
            const float infected_percentage = record.number_of_infected_people / m_config->general.number_of_people;
            m_system_saturated = infected_percentage > m_config->general.saturation_level;
        }
    }

    inline void CSimulation::Update_Statistics(const TStatistics_Record& record)
    {
        // Update the statics.
        m_statistics.number_of_susceptible_people.emplace_back(record.number_of_susceptible_people);
        m_statistics.number_of_contractions_per_person.emplace_back(record.number_of_infections_per_person);
        m_statistics.number_of_fatalities.emplace_back(record.number_of_fatalities);
        m_statistics.number_of_immune_people.emplace_back(record.number_of_immune_people);
        m_statistics.number_of_infected_people.emplace_back(record.number_of_infected_people);

        // Each record represents 24 hours (one day).
        m_statistics.time.emplace_back(m_time / 24.0);
    }

    inline void CSimulation::Update_Statistics_Record(TStatistics_Record& record, const CPerson& person, std::size_t infected_count)
    {
        // Calculate the average of how many time each person has been infected with the virus so far.
        record.number_of_infections_per_person += static_cast<double>(infected_count) / static_cast<double>(m_people.size());
        
        // Update the record based on the infection state of the person.
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

            // Should not happen.
            default:
                spdlog::error("Unknown infection state detected");
                break;
        }
    }

    bool CSimulation::Try_To_Infect(const CInfection_Model& infected_person_manager, const CInfection_Model& vulnerable_person_manager) const
    {
        // Retrieve the two people.
        const auto person1 = infected_person_manager.Get_Person();
        const auto person2 = vulnerable_person_manager.Get_Person();

        // Get their current infection state.
        const auto person1_state = person1->Get_Infection_State();
        const auto person2_state = person2->Get_Infection_State();

        // Make sure that one of them is infected and the other one is not.
        if (!((person1_state == CPerson::NInfection_State::Susceptible && person2_state == CPerson::NInfection_State::Infected) ||
              (person2_state == CPerson::NInfection_State::Susceptible && person1_state == CPerson::NInfection_State::Infected)))
        {
            return false;
        }

        // Make sure their are in a close distance of each other.
        if ((person1->Get_Current_Location() - person2->Get_Current_Location()) > m_config->disease.transmission_distance)
        {
            return false;
        }

        // Make sure they are both at home, or on outside.
        if (person1->Is_Home() ^ person2->Is_Home())
        {
            return false;
        }

        // If they are at home.
        if (person1->Is_Home())
        {
            return utils::Try_Event(m_config->disease.transmission_prob_at_home);
        }

        // Both of them are outside.
        return utils::Try_Event(m_config->disease.transmission_prob_on_move);
    }
}

// EOF