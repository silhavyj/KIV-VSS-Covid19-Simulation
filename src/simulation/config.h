#pragma once

#include <cstddef>

namespace kiv_vss
{
    /// General configuration of the simulation
    struct TGeneral
    {
        std::size_t number_of_people{500};                  ///< Total number of people
        std::size_t number_of_initially_infected_people{5}; ///< Number of initially infected people
        float ratio_of_people_in_self_isolation{0.5};       ///< Percentage of people in self-isolation
        float saturation_level{0.2};                        ///< Percentage of when the health care system becomes saturated
        float world_size{2000};                             ///< Square dimensions of the world
    };

    /// Configuration regarding the disease
    struct TDisease
    {
        float transmission_distance{5};           ///< Maximum transmission distance
        float transmission_prob_on_move{0.5};     ///< Probability of the virus transmission outside (e.g. a restaurant)
        float transmission_prob_at_home{0.5};     ///< Probability of the virus transmission at home
        float death_prob{0.15};                   ///< Death probability when the health care system is NOT saturated
        float death_saturated_prob{0.3};          ///< Death probability when the health care system is saturated
        float self_isolating_when_infected{0.5};  ///< Probability of a person going in self-isolation when infected 

        std::size_t average_infection_period{14}; ///< Average infection period [days]
        std::size_t average_immunity_period{60};  ///< Average immunity period [days]

        /// Returns the average infection period in hours.
        /// \return Average infection period [hours]
        [[nodiscard]] inline std::size_t Get_Average_Infection_Period() const noexcept
        {
            return 24 * average_infection_period;
        }

        /// Returns the average immunity period in hours.
        /// \return Average immunity period [hours]
        [[nodiscard]] inline std::size_t Get_Average_Immunity_Period() const noexcept
        {
            return 24 * average_immunity_period;
        }
    };

    /// Configuration regarding mobility
    struct TMobility
    {
        float average_person_speed = 10;             ///< Average speed of a person
        float variance_person_speed = 1;             ///< Variance of a person's speed
        std::size_t max_hours_spent_at_location = 8; ///< Maximum number of hours a person is allowed to stay at a location

        /// Configuration regarding commuting
        struct TTransition
        {
            float go_to_popular_location_prob; ///< Probability of going to a popular location (e.g. a pub)
            float go_to_random_location_prob;  ///< Probability of going to an arbitrary location
            float go_home_prob;                ///< Probability of going home
        };

        TTransition isolation     { 0.001, 0.001, 0.998 }; ///< Transition probabilities when a person is self-isolating
        TTransition non_isolation { 0.4,   0.2,   0.4   }; ///< Transition probabilities when a person is NOT self-isolating
    };

    /// Configuration of the simulation
    struct TConfig
    {
        TGeneral general;   ///< General configuration
        TDisease disease;   ///< Disease configuration
        TMobility mobility; ///< Mobility configuration
    };
}

// EOF