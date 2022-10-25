#include <cassert>

#include "spdlog/spdlog.h"

#include "simulator.h"
#include "singleton.h"
#include "utils.h"

namespace kiv_vss
{
    CSimulator::CSimulator()
        : m_config(Singleton<TConfig>::Get_Instance())
    {
        Generate_Popular_Locations();
        Generate_People();
    }

    void CSimulator::Update()
    {
        Move_People_Around();
        Spread_Virus();
    }

    const std::vector<CPerson>& CSimulator::Get_People() const
    {
        return m_people;
    }

    size_t CSimulator::Get_Number_Of_Infected_People() const
    {
        return m_infected_people_mngs.size();
    }

    size_t CSimulator::Get_Number_Of_Fatalities() const
    {
        return m_fatalities.size();
    }

    const std::vector<CLocation>& CSimulator::Get_Popular_Locations() const
    {
        return m_popular_locations;
    }

    void CSimulator::Generate_Popular_Locations()
    {
        for (size_t i = 0; i < m_config->Number_Of_Popular_Locations; ++i)
        {
            m_popular_locations.emplace_back(Generate_Random_Downtown_Location());
        }
    }

    void CSimulator::Generate_People()
    {
        auto number_of_people_in_self_isolation = static_cast<size_t>(
            static_cast<double>(m_config->Number_Of_People) * m_config->Ratio_Of_People_In_Self_Isolation
        );

        m_people.reserve(m_config->Number_Of_People);
        m_infection_managers.reserve(m_config->Number_Of_People);

        for (size_t i = 0; i < m_config->Number_Of_People; ++i)
        {
            m_people.emplace_back(Generate_Random_Location());
            const bool self_isolating = i < number_of_people_in_self_isolation;
            m_mobility_managers.emplace_back(&m_people[i], self_isolating, &m_popular_locations);
        }

        size_t number_of_infected_people = 0;
        for (size_t i = 0; i < m_config->Number_Of_People; ++i)
        {
            if (!m_mobility_managers[i].Is_Self_Isolating() &&
                number_of_infected_people < m_config->Number_Of_Initially_Infected_People)
            {
                m_infection_managers.emplace_back(&m_people[i], true);
                m_infected_people_mngs.insert(&m_infection_managers.back());
                ++number_of_infected_people;
            }
            else
            {
                m_infection_managers.emplace_back(&m_people[i], false);
                m_vulnerable_people_mngs.insert(&m_infection_managers.back());
            }
            m_mobility_managers_map[&m_people[i]] = &m_infection_managers.back();
        }
    }

    void CSimulator::Move_People_Around()
    {
        for (auto& manager : m_mobility_managers)
        {
            manager.Update();

            const auto person = manager.Get_Person();
            auto infection_manager = m_mobility_managers_map[person];
            infection_manager->Update();

            if (!person->Is_Alive())
            {
                m_fatalities.insert(person);
            }

            if (person->Is_Infected() && !m_infected_people_mngs.count(infection_manager))
            {
                m_infected_people_mngs.insert(infection_manager);
            }
            if (!person->Is_Infected() && m_infected_people_mngs.count(infection_manager))
            {
                m_infected_people_mngs.erase(infection_manager);
            }
            if (person->Is_Vulnerable() && !m_vulnerable_people_mngs.count(infection_manager))
            {
                m_vulnerable_people_mngs.insert(infection_manager);
            }
            if (!person->Is_Vulnerable() && m_vulnerable_people_mngs.count(infection_manager))
            {
                m_vulnerable_people_mngs.erase(infection_manager);
            }
        }
    }

    void CSimulator::Spread_Virus()
    {
        for (auto& infected_people_mng : m_infected_people_mngs)
        {
            for (auto& vulnerable_people_mng : m_vulnerable_people_mngs)
            {
                if (Infects(infected_people_mng, vulnerable_people_mng))
                {
                    vulnerable_people_mng->Infect();
                }
            }
        }
    }

    bool CSimulator::Infects(const CInfection_Manger* infected_person_manager, const CInfection_Manger* vulnerable_person_manager) const
    {
        const auto infected_person = infected_person_manager->Get_Person();
        const auto vulnerable_person = vulnerable_person_manager->Get_Person();

        if (!infected_person->Is_Infected() || !vulnerable_person->Is_Vulnerable())
        {
            return false;
        }
        if ((infected_person->Get_Current_Location() - vulnerable_person->Get_Current_Location()) > m_config->Disease_Transmission_Distance)
        {
            return false;
        }
        if (infected_person->Is_Home() ^ vulnerable_person->Is_Home())
        {
            return false;
        }
        if (infected_person->Is_Home() && vulnerable_person->Is_Home())
        {
            return utils::Try_Event(m_config->Disease_Transmission_Probability_At_Home);
        }
        return utils::Try_Event(m_config->Disease_Transmission_Probability_On_Move);
    }

    inline CLocation CSimulator::Generate_Random_Downtown_Location() const
    {
        const auto Get_Random_Downtown_Axis = [&]()
        {
            double axis;
            do
            {
                axis = utils::Random<std::normal_distribution<double>>(
                    m_config->Downtown_Center_Location,
                    m_config->Downtown_Variance
                );
            } while (axis < 0.0 || axis > m_config->World_Size);

            return axis;
        };

        const double y = Get_Random_Downtown_Axis();
        const double x = Get_Random_Downtown_Axis();

        return CLocation(y, x);
    }

    inline CLocation CSimulator::Generate_Random_Location() const
    {
        const auto Get_Random_Axis = [&]()
        {
            return utils::Random<std::uniform_real_distribution<double>>(0, m_config->World_Size);
        };

        const double y = Get_Random_Axis();
        const double x = Get_Random_Axis();

        return CLocation(y, x);
    }
}