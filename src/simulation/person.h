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
        explicit CPerson(const CLocation& location);
        ~CPerson() = default;

        [[nodiscard]] CLocation& Get_Current_Location();
        [[nodiscard]] const CLocation& Get_Home_Location() const;
        [[nodiscard]] bool Is_Home() const;

        [[nodiscard]] bool Is_Alive() const;
        [[nodiscard]] bool Is_Infected() const;
        [[nodiscard]] bool Is_Vulnerable() const;
        [[nodiscard]] bool Is_Immune() const;

        void Set_Infection_State(NInfection_State infection_state);

    private:
        CLocation m_home;
        CLocation m_current_location;
        NInfection_State m_infection_state;
    };
}