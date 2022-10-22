#include "simulator.h"
#include "utils.h"
#include "config.h"
#include "singleton.h"

namespace kiv_vss
{
    CSimulator::CSimulator()
        : m_number_of_fatalities(0),
          m_number_of_immune_people(0)
    {
        Generate_Popular_Locations(kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Number_Of_Popular_Places);
        Generate_People(kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Number_Of_People);
    }

    const std::vector<CLocation>& CSimulator::Get_Popular_Locations() const
    {
        return m_popular_locations;
    }

    const std::vector<CPerson>& CSimulator::Get_People() const
    {
        return m_people;
    }

    size_t CSimulator::Get_Number_Of_Infected_People() const
    {
        return m_number_of_infected_people;
    }

    size_t CSimulator::Get_Number_Of_Fatalities() const
    {
        return m_number_of_fatalities;
    }

    size_t CSimulator::Get_Number_Immune_People() const
    {
        return m_number_of_immune_people;
    }

    void CSimulator::Generate_Popular_Locations(const size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            m_popular_locations.emplace_back(Generate_Random_Downtown_Location());
        }
    }

    void CSimulator::Generate_People(const size_t count)
    {
        const auto number_of_people_in_self_isolation = static_cast<size_t>(
            kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Number_Of_People * kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Ratio_Of_People_In_Self_Isolation
        );

        for (size_t i = 0; i < count; ++i)
        {
            const CLocation home = Generate_Random_Location();
            const bool self_isolating = i < number_of_people_in_self_isolation;
            m_people.emplace_back(home, self_isolating);
        }
        for (auto& person : m_people)
        {
            m_person_mobility.emplace_back(&person, &m_popular_locations);
        }
        for (size_t i = 0; i < kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Number_Of_Initially_Infected_People; ++i)
        {
            auto person_it = m_people.end();
            std::advance(person_it, -(i + 1));
            person_it->Infect();
        }
        m_number_of_infected_people = kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Number_Of_Initially_Infected_People;
    }

    void CSimulator::Move_People_Around()
    {
        for (auto& mobility : m_person_mobility)
        {
            mobility.Update();
            mobility.Get_Person()->Progress_Infection();
        }
    }

    void CSimulator::Spread_Virus()
    {
        const size_t size = m_people.size();

        m_number_of_infected_people = 0;
        m_number_of_fatalities = 0;
        m_number_of_immune_people = 0;

        for (size_t i = 0; i < size; ++i)
        {
            for (size_t j = i + 1; j < size; ++j)
            {
                auto& person_i = m_people.at(i);
                auto& person_j = m_people.at(j);

                if (Infects(person_i, person_j))
                {
                    if (person_i.Is_Vulnerable())
                    {
                        person_i.Infect();
                    }
                    else
                    {
                        person_j.Infect();
                    }
                }
            }
            if (m_people[i].Is_Infected())
            {
                ++m_number_of_infected_people;
            }
            if (!m_people[i].Is_Alive())
            {
                ++m_number_of_fatalities;
            }
            if (m_people[i].Is_Immune())
            {
                ++m_number_of_immune_people;
            }
        }
    }

    void CSimulator::Update()
    {
        Move_People_Around();
        Spread_Virus();
    }

    inline bool CSimulator::Infects(CPerson& person_i, CPerson& person_j)
    {
        return ((((person_i.Is_Infected() && person_j.Is_Vulnerable()) || ((person_i.Is_Vulnerable() && person_j.Is_Infected())))) &&
                (!person_i.Is_Self_Isolating() && !person_j.Is_Self_Isolating()) &&
                ((person_i.Get_Current_Location() - person_j.Get_Current_Location()) < kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Disease_Transmission_Distance) &&
                (utils::Try_Event(kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Disease_Transmission_Probability)));
    }

    inline CLocation CSimulator::Generate_Random_Downtown_Location()
    {
        const auto Get_Random_Downtown_Axis = []()
        {
            double axis;
            do
            {
                axis = utils::Random<std::normal_distribution<double>>(
                    kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Downtown_Location,
                    kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Downtown_Variance
                );
            } while (axis < 0.0 || axis > kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->World_Size);
            return axis;
        };

        const double y = Get_Random_Downtown_Axis();
        const double x = Get_Random_Downtown_Axis();

        return CLocation(y, x);
    }

    inline CLocation CSimulator::Generate_Random_Location()
    {
        const auto Get_Random_Axis = []()
        {
            return utils::Random<std::uniform_real_distribution<double>>(0, kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->World_Size);
        };

        const double y = Get_Random_Axis();
        const double x = Get_Random_Axis();

        return CLocation(y, x);
    }
}