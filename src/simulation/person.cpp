#include "utils.h"
#include "person.h"
#include "singleton.h"

namespace kiv_vss
{
    CPerson::CPerson(const CLocation& home)
        : m_home_location(home),
          m_current_location(home),
          m_infection_state{NInfection_State::Susceptible},
          m_config{Singleton<TConfig>::Get_Instance()},
          m_speed(Generate_Random_Speed())
    {

    }


    const CLocation& CPerson::Get_Current_Location() const noexcept
    {
        return m_current_location;
    }

    const CLocation& CPerson::Get_Home_Location() const noexcept
    {
        return m_home_location;
    }

    bool CPerson::Is_Home() const
    {
        return m_current_location == m_home_location;
    }

    void CPerson::Set_Infection_State(NInfection_State infection_state) noexcept
    {
        m_infection_state = infection_state;
    }

    CPerson::NInfection_State CPerson::Get_Infection_State() const noexcept
    {
        return m_infection_state;
    }

    void CPerson::Move_Toward(const CLocation& location)
    {
        m_current_location.Move_Toward(location, m_speed);
    }

    inline double CPerson::Generate_Random_Speed() const
    {
        double speed;

        // Make sure speed is a positive number.
        do
        {
            speed = kiv_vss::utils::Random<std::normal_distribution<>>(
                m_config->mobility.average_person_speed,
                m_config->mobility.variance_person_speed
            );
        } while (speed < 0.0);

        return speed;
    }
}

// EOF
