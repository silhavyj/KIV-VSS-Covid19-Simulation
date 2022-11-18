#include "spdlog/spdlog.h"

#include "utils.h"
#include "singleton.h"
#include "mobility_model.h"

namespace kiv_vss
{
    CMobility_Model::CMobility_Model(CPerson* person, const std::vector<CLocation>* popular_places, bool self_isolating)
        : m_person{person},
          m_popular_places(popular_places),
          m_self_isolating{self_isolating},
          m_self_isolating_due_to_infection{false},
          m_config{Singleton<TConfig>::Get_Instance()},
          m_person_is_moving{false},
          m_next_location(Pick_Next_Location()),
          m_at_location_countdown(Generate_Random_Hours_At_Location())
    {

    }

    void CMobility_Model::Update()
    {
        if (m_person->Get_Infection_State() == CPerson::NInfection_State::Dead)
        {
            return;
        }
        if (m_person_is_moving)
        {
            m_person->Move_Toward(m_next_location);
            if (m_person->Get_Current_Location() == m_next_location)
            {
                m_person_is_moving = false;
                m_at_location_countdown = Generate_Random_Hours_At_Location();
            }
        }
        else
        {
            if (0 != m_at_location_countdown)
            {
                --m_at_location_countdown;
            }
            else
            {
                Check_Infection_State();
                m_next_location = Pick_Next_Location();
                m_person_is_moving = true;
            }
        }
    }

    void CMobility_Model::Check_Infection_State()
    {
        static bool seen_infected = false;
        const auto person_is_infected = m_person->Get_Infection_State() == CPerson::NInfection_State::Infected;

        if (!seen_infected && person_is_infected)
        {
            seen_infected = true;
            m_self_isolating_due_to_infection = utils::Try_Event(m_config->disease.self_isolating_when_infected);
        }
        if (seen_infected && !person_is_infected)
        {
            seen_infected = false;
            m_self_isolating_due_to_infection = false;
        }
    }

    CLocation CMobility_Model::Pick_Next_Location() const
    {
        const auto Construct_Transition_Probabilities = [&](const TMobility::TTransition& transition)
        {
            std::vector<double> probabilities = {
                transition.go_to_random_location_prob,
                transition.go_to_popular_location_prob,
                transition.go_home_prob
            };
            return probabilities;
        };

        std::vector<double> random_events;
        if (m_self_isolating || m_self_isolating_due_to_infection)
        {
            random_events = Construct_Transition_Probabilities(m_config->mobility.isolation);
        }
        else
        {
            random_events = Construct_Transition_Probabilities(m_config->mobility.non_isolation);
        }

        const auto event = static_cast<std::uint8_t>(utils::Pick_Event(random_events));
        const auto action = static_cast<NMobility_State>(event);
        CLocation next_location{0,0};

        switch (action)
        {
            case NMobility_State::Headed_To_Random_Location:
                next_location = CLocation::Generate_Random_In_Square_Location(0, m_config->layout.world_size);
                break;

            case NMobility_State::Headed_To_Popular_Place:
                next_location = Pick_Random_Popular_Place();
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

    inline CLocation CMobility_Model::Pick_Random_Popular_Place() const
    {
        if (m_popular_places->empty())
        {
            return CLocation::Generate_Random_In_Square_Location(0, m_config->layout.world_size);
        }
        const size_t index = utils::Random<std::uniform_int_distribution<size_t>>(0, m_popular_places->size() - 1);
        return m_popular_places->at(index);
    }

    inline std::size_t CMobility_Model::Generate_Random_Hours_At_Location() const
    {
        return utils::Random<std::uniform_int_distribution<size_t>>(0, m_config->mobility.max_hours_spent_at_location);
    }
}