#include "spdlog/spdlog.h"

#include "utils.h"
#include "singleton.h"
#include "mobility_manager.h"

namespace kiv_vss
{
    CMobility_Manager::CMobility_Manager(kiv_vss::CPerson *person, bool self_isolating, const std::vector<CLocation> *popular_locations)
        : m_person(person),
          m_self_isolating(self_isolating),
          m_popular_locations(popular_locations),
          m_config(Singleton<TConfig>::Get_Instance()),
          m_speed(Generate_Random_Speed()),
          m_moving(false),
          m_next_location(Pick_Next_Location()),
          m_at_location_counter(Generate_Random_Hours_At_Location()),
          m_is_infected(m_person->Is_Infected()),
          m_self_isolating_due_to_infection(false)
    {

    }

    void CMobility_Manager::Update()
    {
        if (!m_person->Is_Alive())
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
            if (0 != m_at_location_counter)
            {
                --m_at_location_counter;
            }
            else
            {
                Check_Infection_State();
                m_next_location = Pick_Next_Location();
                m_moving = true;
            }
        }
    }

    CPerson* CMobility_Manager::Get_Person() const
    {
        return m_person;
    }

    bool CMobility_Manager::Is_Self_Isolating() const
    {
        return m_self_isolating;
    }

    void CMobility_Manager::Check_Infection_State()
    {
        if (!m_is_infected && m_person->Is_Infected())
        {
            m_is_infected = true;
            m_self_isolating_due_to_infection = utils::Try_Event(m_config->Self_Isolating_When_Infected);
        }
        if (m_is_infected && !m_person->Is_Infected())
        {
            m_self_isolating = false;
            m_self_isolating_due_to_infection = false;
        }
    }

    CLocation CMobility_Manager::Pick_Next_Location() const
    {
        std::vector<double> random_events;

        if (m_self_isolating_due_to_infection)
        {
            random_events.emplace_back(m_config->Go_To_Random_Location_Probability_4);
            random_events.emplace_back(m_config->Go_To_Popular_Location_Probability_4);
            random_events.emplace_back(m_config->Go_Home_Probability_4);
        }
        else if (m_self_isolating)
        {
            random_events.emplace_back(m_config->Go_To_Random_Location_Probability_3);
            random_events.emplace_back(m_config->Go_To_Popular_Location_Probability_3);
            random_events.emplace_back(m_config->Go_Home_Probability_3);
        }
        else if (m_person->Is_Home())
        {
            random_events.emplace_back(m_config->Go_To_Random_Location_Probability_2);
            random_events.emplace_back(m_config->Go_To_Popular_Location_Probability_2);
        }
        else
        {
            random_events.emplace_back(m_config->Go_To_Popular_Location_Probability_1);
            random_events.emplace_back(m_config->Go_To_Random_Location_Probability_1);
            random_events.emplace_back(m_config->Go_Home_Probability_1);
        }

        const auto event = static_cast<uint8_t>(utils::Pick_Event(random_events));
        const auto action = static_cast<NMobility_State>(event);
        CLocation next_location{0,0};

        switch (action)
        {
            case NMobility_State::Headed_To_Random_Location:
                next_location = Generate_Random_Location();
                break;

            case NMobility_State::Headed_To_Popular_Location:
                if (!m_popular_locations->empty())
                {
                    next_location = Pick_Random_Popular_Location();
                }
                else
                {
                    next_location = Generate_Random_Location();
                }
                break;

            case NMobility_State::Headed_Home:
                next_location = m_person->Get_Home_Location();
                break;

            default:
                spdlog::error("ERROR: Pick_Event did not return a valid NMobility_State. Returning [0; 0] as the default position.");
                break;
        }
        return next_location;
    }

    inline CLocation CMobility_Manager::Pick_Random_Popular_Location() const
    {
        const size_t index = utils::Random<std::uniform_int_distribution<size_t>>(0, m_popular_locations->size() - 1);
        return m_popular_locations->at(index);
    }

    inline CLocation CMobility_Manager::Generate_Random_Location() const
    {
        const auto Get_Random_Axis = [&]()
        {
            return utils::Random<std::uniform_real_distribution<double>>(0, m_config->World_Size);
        };

        const double y = Get_Random_Axis();
        const double x = Get_Random_Axis();

        return CLocation(y, x);
    }

    inline size_t CMobility_Manager::Generate_Random_Hours_At_Location() const
    {
        return utils::Random<std::uniform_int_distribution<size_t>>(0, m_config->Max_Hours_Spent_At_Location);
    }

    inline double CMobility_Manager::Generate_Random_Speed() const
    {
        double speed;
        do
        {
            speed = kiv_vss::utils::Random<std::normal_distribution<>>(
                m_config->Average_Person_Mobility_Speed,
                m_config->Variance_Person_Mobility_Speed
            );
        } while (speed < 0.0);
        return speed;
    }
}