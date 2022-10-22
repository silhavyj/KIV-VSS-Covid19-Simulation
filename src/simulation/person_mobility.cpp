#include <cassert>

#include "person_mobility.h"
#include "utils.h"
#include "config.h"
#include "singleton.h"

namespace kiv_vss
{
    CPerson_Mobility::CPerson_Mobility(CPerson* person, const std::vector<CLocation>* popular_locations)
        : m_person(person),
          m_popular_locations(popular_locations),
          m_speed(Generate_Random_Speed()),
          m_state(NMobility_State::Headed_Home),
          m_moving(false),
          m_next_location(Generate_Random_Location()),
          m_at_location_counter(Generate_Random_Hours_At_Location())
    {

    }

    void CPerson_Mobility::Update()
    {
        if (m_person->Is_Self_Isolating() || !m_person->Is_Alive())
        {
            return;
        }
        if (m_moving)
        {
            m_person->Get_Current_Location().Move_Toward(m_next_location, m_speed);
            if (m_person->Get_Current_Location() == m_next_location)
            {
                m_moving = false;
                m_at_location_counter = Generate_Random_Hours_At_Location();
            }
        }
        else
        {
            if (m_at_location_counter != 0)
            {
                --m_at_location_counter;
            }
            else
            {
                Pick_Next_Location();
                m_moving = true;
            }
        }
    }

    CPerson* CPerson_Mobility::Get_Person() const
    {
        return m_person;
    }

    void CPerson_Mobility::Pick_Next_Location()
    {
        std::vector<double> random_events;

        if (m_person->Is_Home())
        {
            random_events.emplace_back(kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Go_To_Popular_Location_Probability_2);
            random_events.emplace_back(kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Go_To_Random_Location_Probability_2);
        }
        else
        {
            random_events.emplace_back(kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Go_To_Popular_Location_Probability_1);
            random_events.emplace_back(kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Go_To_Random_Location_Probability_1);
            random_events.emplace_back(kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Go_Home_Probability_1);
        }

        const auto event = static_cast<uint8_t>(utils::Pick_Event(random_events));
        const auto action = static_cast<NMobility_State>(event);

        switch (action)
        {
            case NMobility_State::Headed_To_Random_Location:
                m_next_location = Generate_Random_Location();
                break;

            case NMobility_State::Headed_To_Popular_Location:
                m_next_location = Generate_Random_Popular_Location();
                break;

            case NMobility_State::Headed_Home:
                m_next_location = m_person->Get_Home_Location();
                break;
        }
    }

    inline CLocation CPerson_Mobility::Generate_Random_Popular_Location() const
    {
        size_t index = utils::Random<std::uniform_int_distribution<size_t>>(0, m_popular_locations->size() - 1);
        return m_popular_locations->at(index);
    }

    inline double CPerson_Mobility::Generate_Random_Speed()
    {
        double speed;
        do
        {
            speed = utils::Random<std::normal_distribution<double>>(
                kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Average_Person_Mobility_Speed,
                kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Person_Mobility_Speed_Variance
            );
        } while (speed < 0.0);
        return speed;
    }

    inline size_t CPerson_Mobility::Generate_Random_Hours_At_Location()
    {
        return utils::Random<std::uniform_int_distribution<size_t>>(
            0,
            kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Max_Hours_Spent_At_Location
        );
    }

    inline CLocation CPerson_Mobility::Generate_Random_Location()
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