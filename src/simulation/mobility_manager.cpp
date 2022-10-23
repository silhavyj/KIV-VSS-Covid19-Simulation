#include "utils.h"
#include "singleton.h"
#include "mobility_manager.h"

namespace kiv_vss
{
    CMobility_Manager::CMobility_Manager(kiv_vss::CPerson *person, bool self_isolating, const std::vector<CLocation> *popular_locations)
        : m_person(person),
          m_self_isolating(self_isolating),
          m_popular_locations(popular_locations),
          m_config(Singleton<Config>::Get_Instance()),
          m_speed(Generate_Random_Speed()),
          m_state(NMobility_State::At_Home),
          m_moving(false),
          m_next_location(Pick_Next_Location()),
          m_at_location_counter(Generate_Random_Hours_At_Location())
    {

    }

    void CMobility_Manager::Update()
    {

    }

    CLocation CMobility_Manager::Pick_Next_Location() const
    {

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
        return kiv_vss::utils::Random<std::poisson_distribution<size_t>>(m_config->Average_Person_Mobility_Speed);
    }
}