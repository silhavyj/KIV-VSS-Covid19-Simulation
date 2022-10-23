#pragma once

#include <vector>

#include "person.h"

namespace kiv_vss
{
    class CPerson_Mobility
    {
    public:
        explicit CPerson_Mobility(CPerson* person, const std::vector<CLocation>* popular_locations);
        ~CPerson_Mobility() = default;

        void Update();
        CPerson* Get_Person() const;

    private:
        CLocation Pick_Next_Location();
        CLocation Generate_Random_Popular_Location() const;

        static double Generate_Random_Speed();
        static size_t Generate_Random_Hours_At_Location();
        static CLocation Generate_Random_Location();

    private:
        enum class NMobility_State : uint8_t
        {
            Headed_To_Random_Location,
            Headed_To_Popular_Location,
            Headed_Home
        };

    public:
        CPerson* m_person;
        const std::vector<CLocation>* m_popular_locations;
        double m_speed;
        NMobility_State m_state;
        bool m_moving;
        CLocation m_next_location;
        size_t m_at_location_counter;
    };
}