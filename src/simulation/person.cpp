#include <cassert>

#include "person.h"
#include "config.h"
#include "utils.h"
#include "singleton.h"

namespace kiv_vss
{
    CPerson::CPerson(const CLocation& location, const bool self_isolating)
        : m_home(location),
          m_current_location(location),
          m_self_isolating(self_isolating),
          m_infection_state(NInfection_State::Vulnerable),
          m_counter(0),
          m_infection_counter(0)
    {

    }

    CLocation& CPerson::Get_Current_Location()
    {
        return m_current_location;
    }

    const CLocation& CPerson::Get_Home_Location() const
    {
        return m_home;
    }

    bool CPerson::Is_Alive() const
    {
        return NInfection_State::Dead != m_infection_state;
    }

    bool CPerson::Is_Infected() const
    {
        return NInfection_State::Infected == m_infection_state;
    }

    bool CPerson::Is_Vulnerable() const
    {
        return NInfection_State::Vulnerable == m_infection_state;
    }

    bool CPerson::Is_Immune() const
    {
        return NInfection_State::Immune == m_infection_state;
    }

    bool CPerson::Is_Self_Isolating() const
    {
        return m_self_isolating;
    }

    bool CPerson::Is_Home() const
    {
        return m_current_location == m_home;
    }

    CPerson::NInfection_State CPerson::Get_Infection_State() const
    {
        return m_infection_state;
    }

    void CPerson::Infect()
    {
        m_infection_state = NInfection_State::Infected;
        m_counter = Generate_Random_Infection_Period();
        ++m_infection_counter;
        assert(m_counter < 5000);
    }

    void CPerson::Progress_Infection()
    {
        if (0 != m_counter)
        {
            --m_counter;
        }
        else
        {
            if (Is_Immune())
            {
                m_infection_state = NInfection_State::Vulnerable;
            }
            else if (Is_Infected())
            {
                if (utils::Try_Event(kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Death_Probability))
                {
                    m_infection_state = NInfection_State::Dead;
                }
                else
                {
                    m_infection_state = NInfection_State::Immune;
                    m_counter = Generate_Random_Immunity_Period();
                }
            }
        }
    }

    inline size_t CPerson::Generate_Random_Infection_Period()
    {
        double value;
        do
        {
            value = utils::Random<std::normal_distribution<double>>(
                kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Average_Infection_Period,
                kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Infection_Period_Variance
            );
        } while (value < 0.0);
        return static_cast<size_t>(value);
    }

    inline size_t CPerson::Generate_Random_Immunity_Period()
    {
        double value;
        do
        {
            value = utils::Random<std::normal_distribution<double>>(
                kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Covid_Immunity_Period,
                kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Covid_Immunity_Variance
            );
        } while (value < 0.0);
        return static_cast<size_t>(value);
    }
}