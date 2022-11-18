#pragma once

#include <cstddef>

namespace kiv_vss
{
    struct TDisease
    {
        double transmission_distance{5};
        double transmission_prob_on_move{0.65};
        double transmission_prob_at_home{0.5};
        double death_prob{0.15};
        double self_isolating_when_infected{0.4};
        std::size_t average_infection_period{24 * 14};
        std::size_t average_immunity_period{24 * 60};
    };

    struct TLayout
    {
        double world_size{5000};
        double ratio_of_people_in_self_isolation{0.5};
        std::size_t number_of_people{1000};
        std::size_t number_of_initially_infected_people{5};
    };

    struct TMobility
    {
        double average_person_speed = 10;
        double variance_person_speed = 1;
        std::size_t max_hours_spent_at_location = 8;

        struct TTransition
        {
            double go_to_popular_location_prob;
            double go_to_random_location_prob;
            double go_home_prob;
        };

        TTransition isolation     { 0.001, 0.001, 0.998 };
        TTransition non_isolation { 0.4,   0.2,   0.4   };
    };

    struct TConfig
    {
        TLayout layout;
        TDisease disease;
        TMobility mobility;
    };
}