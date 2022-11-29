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
        // If the person has been chosen to be 
        // initially infected, infect them.
        if (initially_infected)
        {
            Infect();
        }
    }

    void CInfection_Model::Infect()
    {
        // Change the state of the person.
        m_person->Set_Infection_State(CPerson::NInfection_State::Infected);

        // Generate a random period of time for which the infection will last.
        m_counter = Generate_Random_Infection_Period();

        // Increment the counter indicating how much
        // time the person has had the virus.
        ++m_infection_counter;
    }

    void CInfection_Model::Update()
    {
        // Decrement the counter, unless it has hit 0.
        if (0 != m_counter)
        {
            --m_counter;
        }
        else
        {
            // Get the current state of the person.
            const auto infection_state = m_person->Get_Infection_State();

            switch (infection_state)
            {
                // If they are immune, make them susceptible again.
                case CPerson::NInfection_State::Immune:
                    m_person->Set_Infection_State(CPerson::NInfection_State::Susceptible);
                    break;

                // If they are infected
                case CPerson::NInfection_State::Infected:
                    // Roll dice whether the person will die or develop immunity
                    // (depending on whether the heath care system is saturated or not).
                    if (utils::Try_Event(*m_system_saturated ? m_config->disease.death_saturated_prob : m_config->disease.death_prob))
                    {
                        // The person dies.
                        m_person->Set_Infection_State(CPerson::NInfection_State::Dead);
                    }
                    else
                    {
                        // The person develops immunity.
                        m_person->Set_Infection_State(CPerson::NInfection_State::Immune);

                        // Generate a random period of time for which the immunity will last.
                        m_counter = Generate_Random_Immunity_Period();
                    }
                    break;

                // There is nothing to be done.
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

// EOF