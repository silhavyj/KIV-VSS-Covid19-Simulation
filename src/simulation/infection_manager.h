#pragma once

#include "person.h"
#include "config.h"

namespace kiv_vss
{
    class CInfection_Manger
    {
    public:
        explicit CInfection_Manger(CPerson* person, bool initially_infected);
        ~CInfection_Manger() = default;

        void Infect();
        void Update();
        [[nodiscard]] CPerson* Get_Person() const;
        [[nodiscard]] size_t Get_Infection_Count() const;

    private:
        [[nodiscard]] size_t Generate_Random_Infection_Period() const;
        [[nodiscard]] size_t Generate_Random_Immunity_Period() const;

    private:
        CPerson* m_person;
        const TConfig* m_config;
        size_t m_infection_counter;
        size_t m_counter;
    };
}