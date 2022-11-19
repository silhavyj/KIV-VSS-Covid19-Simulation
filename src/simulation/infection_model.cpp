#include "utils.h"
#include "singleton.h"
#include "infection_model.h"

namespace kiv_vss
{
    CInfection_Model::CInfection_Model(CPerson* person, bool* system_saturated, bool initially_infected)
        : m_person{person},
          m_config{Singleton<TConfig>::Get_Instance()},
          m_infection_counter{0},
          m_counter{0},
          m_system_saturated{system_saturated}
    {
        if (initially_infected)
        {
            Infect();
        }
    }

    void CInfection_Model::Infect()
    {
        m_person->Set_Infection_State(CPerson::NInfection_State::Infected);
        m_counter = Generate_Random_Infection_Period();
        ++m_infection_counter;
    }

    void CInfection_Model::Update()
    {
        if (0 != m_counter)
        {
            --m_counter;
        }
        else
        {
            const auto infection_state = m_person->Get_Infection_State();
            switch (infection_state)
            {
                case CPerson::NInfection_State::Immune:
                    m_person->Set_Infection_State(CPerson::NInfection_State::Susceptible);
                    break;

                case CPerson::NInfection_State::Infected:
                    if (utils::Try_Event(*m_system_saturated ? m_config->disease.death_saturated_prob : m_config->disease.death_prob))
                    {
                        m_person->Set_Infection_State(CPerson::NInfection_State::Dead);
                    }
                    else
                    {
                        m_person->Set_Infection_State(CPerson::NInfection_State::Immune);
                        m_counter = Generate_Random_Immunity_Period();
                    }
                    break;

                default:
                    break;
            }
        }
    }

    const CPerson* CInfection_Model::Get_Person() const noexcept
    {
        return m_person;
    }

    std::size_t CInfection_Model::Get_Infection_Count() const noexcept
    {
        return m_infection_counter;
    }

    inline std::size_t CInfection_Model::Generate_Random_Infection_Period() const
    {
        return utils::Random<std::poisson_distribution<std::size_t>>(m_config->disease.Get_Average_Infection_Period());
    }

    inline std::size_t CInfection_Model::Generate_Random_Immunity_Period() const
    {
        return utils::Random<std::poisson_distribution<std::size_t>>(m_config->disease.Get_Average_Immunity_Period());
    }
}