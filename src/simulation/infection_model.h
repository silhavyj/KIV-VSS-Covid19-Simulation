#pragma once

#include "person.h"
#include "config.h"

namespace kiv_vss
{
    class CInfection_Model
    {
    public:
        explicit CInfection_Model(CPerson* person, bool initially_infected = false);
        ~CInfection_Model() = default;

        void Infect();
        void Update();
        [[nodiscard]] const CPerson* Get_Person() const;
        [[nodiscard]] std::size_t Get_Infection_Count() const;

    private:
        [[nodiscard]] std::size_t Generate_Random_Infection_Period() const;
        [[nodiscard]] std::size_t Generate_Random_Immunity_Period() const;

    private:
        CPerson* m_person;
        const TConfig* m_config;
        std::size_t m_infection_counter;
        std::size_t m_counter;
    };
}