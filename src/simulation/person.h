#pragma once

#include "location.h"

namespace kiv_vss
{
    class CPerson
    {
    public:
        enum class NInfection_State : uint8_t
        {
            Vulnerable,
            Infected,
            Immune,
            Dead
        };

    public:
        explicit CPerson(const CLocation& location, const bool self_isolating);
        ~CPerson() = default;

        [[nodiscard]] CLocation& Get_Current_Location();
        [[nodiscard]] const CLocation& Get_Home_Location() const;
        [[nodiscard]] bool Is_Alive() const;
        [[nodiscard]] bool Is_Infected() const;
        [[nodiscard]] bool Is_Vulnerable() const;
        [[nodiscard]] bool Is_Immune() const;
        [[nodiscard]] bool Is_Self_Isolating() const;
        [[nodiscard]] bool Is_Home() const;
        [[nodiscard]] NInfection_State Get_Infection_State() const;

        void Infect();
        void Progress_Infection();

    private:
        static size_t Generate_Random_Infection_Period();
        static size_t Generate_Random_Immunity_Period();

    private:
        CLocation m_home;
        CLocation m_current_location;
        bool m_self_isolating;
        NInfection_State m_infection_state;
        size_t m_counter;
        size_t m_infection_counter;
    };
}