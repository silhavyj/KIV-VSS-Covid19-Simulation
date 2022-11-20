#pragma once

#include "config.h"
#include "location.h"

namespace kiv_vss
{
    class CPerson
    {
    public:
        enum class NInfection_State : std::uint8_t
        {
            Susceptible,
            Infected,
            Immune,
            Dead
        };

    public:
        explicit CPerson(const CLocation& home);
        ~CPerson() = default;


        [[nodiscard]] const CLocation& Get_Current_Location() const noexcept;
        [[nodiscard]] const CLocation& Get_Home_Location() const noexcept;
        [[nodiscard]] bool Is_Home() const;
        void Set_Infection_State(NInfection_State infection_state) noexcept;
        [[nodiscard]] NInfection_State Get_Infection_State() const noexcept;
        void Move_Toward(const CLocation& location);

    private:
        [[nodiscard]] double Generate_Random_Speed() const;

    private:
        CLocation m_home_location;
        CLocation m_current_location;
        NInfection_State m_infection_state;
        const TConfig* m_config;
        double m_speed;
    };
}

// EOF