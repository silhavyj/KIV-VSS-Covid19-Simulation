#pragma once

#include <vector>

#include "person.h"
#include "config.h"

namespace kiv_vss
{
    class CMobility_Model
    {
    public:
        explicit CMobility_Model(CPerson* person, const std::vector<CLocation>* popular_places, bool self_isolating);
        ~CMobility_Model() = default;

        void Update();

    private:
        void Check_Infection_State();
        [[nodiscard]] CLocation Pick_Next_Location() const;
        [[nodiscard]] CLocation Pick_Random_Popular_Place() const;
        [[nodiscard]] std::size_t Generate_Random_Hours_At_Location() const;

    private:
        enum class NMobility_State : std::uint8_t
        {
            Headed_To_Random_Location,
            Headed_To_Popular_Place,
            Headed_Home
        };

    private:
        CPerson* m_person;
        const std::vector<CLocation>* m_popular_places;
        bool m_self_isolating;
        bool m_self_isolating_due_to_infection;
        const TConfig* m_config;
        bool m_person_is_moving;
        CLocation m_next_location;
        std::size_t m_at_location_countdown;
    };
}

// EOF