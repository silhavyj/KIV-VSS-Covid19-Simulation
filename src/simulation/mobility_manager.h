#pragma once

#include <vector>

#include "config.h"
#include "person.h"

namespace kiv_vss
{
    class CMobility_Manager
    {
    public:
        explicit CMobility_Manager(CPerson* person, bool self_isolating, const std::vector<CLocation>* popular_locations);
        ~CMobility_Manager() = default;

        void Update();

    private:
        CLocation Pick_Next_Location() const;
        CLocation Pick_Random_Popular_Location() const;
        CLocation Generate_Random_Location() const;
        size_t Generate_Random_Hours_At_Location() const;
        double Generate_Random_Speed() const;

    private:
        enum class NMobility_State : uint8_t
        {
            Headed_To_Random_Location,
            Headed_To_Popular_Location,
            Headed_Home,
            At_Home
        };

    private:
        CPerson* m_person;
        bool m_self_isolating;
        const std::vector<CLocation>* m_popular_locations;
        const Config* m_config;
        double m_speed;
        NMobility_State m_state;
        bool m_moving;
        CLocation m_next_location;
        size_t m_at_location_counter;
    };
}