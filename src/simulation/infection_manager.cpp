#include "spdlog/spdlog.h"

#include "utils.h"
#include "singleton.h"
#include "infection_manager.h"

namespace kiv_vss
{
     CInfection_Manger::CInfection_Manger(CPerson* person, bool initially_infected)
        : m_person(person),
          m_config(Singleton<TConfig>::Get_Instance()),
          m_infection_counter(0),
          m_counter(0)
     {
         if (initially_infected)
         {
             Infect();
         }
     }

    void CInfection_Manger::Infect()
    {
        m_person->Set_Infection_State(CPerson::NInfection_State::Infected);
        m_counter = Generate_Random_Infection_Period();
        ++m_infection_counter;
    }

    CPerson* CInfection_Manger::Get_Person() const
    {
         return m_person;
    }

    size_t CInfection_Manger::Get_Infection_Count() const
    {
        return m_infection_counter;
    }

    void CInfection_Manger::Update()
    {
        if (0 != m_counter)
        {
            --m_counter;
        }
        else
        {
            if (m_person->Is_Immune())
            {
                m_person->Set_Infection_State(CPerson::NInfection_State::Vulnerable);
            }
            else if (m_person->Is_Infected())
            {
                if (utils::Try_Event(m_config->Death_Probability))
                {
                    m_person->Set_Infection_State(CPerson::NInfection_State::Dead);
                }
                else
                {
                    m_person->Set_Infection_State(CPerson::NInfection_State::Immune);
                    m_counter = Generate_Random_Immunity_Period();
                }
            }
        }
    }

    inline size_t CInfection_Manger::Generate_Random_Infection_Period() const
    {
        return utils::Random<std::poisson_distribution<size_t>>(m_config->Average_Infection_Period);
    }

    inline size_t CInfection_Manger::Generate_Random_Immunity_Period() const
    {
        return utils::Random<std::poisson_distribution<size_t>>(m_config->Average_Immunity_Period);
    }
}