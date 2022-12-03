#pragma once

#include <random>

namespace kiv_vss::utils
{
    /// Returns a random number from a given distribution.
    /// \tparam Distribution Distribution used to generate random numbers
    /// \tparam ...Args Arguments passed into the function (parameters of the distribution)
    /// \param ...args Parameters of the distribution
    /// \return Random number from the given distribution
    template<typename Distribution, typename ...Args>
    auto Random(const Args... args)
    {
        // Default random device.
        static std::random_device rand_dev{};

        // Create an instance of the distribution.
        Distribution distribution(args...);

        // Return a random number.
        return distribution(rand_dev);
    }

    /// Tries if an event happens or not based on a given probability.
    /// \param probability Probability the the event happens 
    /// \return Return true, if the event happens, false otherwise
    bool Try_Event(double probability);

    /// Picks a random event based on their probabilities passed in as a parameter
    /// \param probabilities Probabilities of individual events
    /// \return Index of an event that has been picked
    size_t Pick_Event(std::vector<double> probabilities);
}

// EOF