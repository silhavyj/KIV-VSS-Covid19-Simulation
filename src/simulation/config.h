#pragma once

#include <cstddef>

namespace kiv_vss
{
    struct TGeneral
    {
        std::size_t number_of_people{500};
        std::size_t number_of_initially_infected_people{5};
        float ratio_of_people_in_self_isolation{0.5};
        float saturation_level{0.2};
        double world_size{5000};
    };

    struct TDisease
    {
        float transmission_distance{5};
        float transmission_prob_on_move{0.5};
        float transmission_prob_at_home{0.5};
        float death_prob{0.15};
        float death_saturated_prob{0.3};
        float self_isolating_when_infected{0.5};

        std::size_t average_infection_period{14};
        std::size_t average_immunity_period{60};

        [[nodiscard]] inline std::size_t Get_Average_Infection_Period() const noexcept
        {
            return 24 * average_infection_period;
        }

        [[nodiscard]] inline std::size_t Get_Average_Immunity_Period() const noexcept
        {
            return 24 * average_immunity_period;
        }
    };

    struct TMobility
    {
        float average_person_speed = 10;
        float variance_person_speed = 1;
        std::size_t max_hours_spent_at_location = 8;

        struct TTransition
        {
            float go_to_popular_location_prob;
            float go_to_random_location_prob;
            float go_home_prob;
        };

        TTransition isolation     { 0.001, 0.001, 0.998 };
        TTransition non_isolation { 0.4,   0.2,   0.4   };
    };

    struct TConfig
    {
        TGeneral general;
        TDisease disease;
        TMobility mobility;
    };
}