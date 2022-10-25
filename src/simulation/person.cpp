#include "person.h"

namespace kiv_vss
{
    CPerson::CPerson(const CLocation& location)
        : m_home(location),
          m_current_location(location),
          m_infection_state(NInfection_State::Vulnerable)
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

    bool CPerson::Is_Home() const
    {
        return m_current_location == m_home;
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

    void CPerson::Set_Infection_State(NInfection_State infection_state)
    {
        m_infection_state = infection_state;
    }
}