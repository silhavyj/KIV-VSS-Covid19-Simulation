#include "spdlog/spdlog.h"

#include "utils.h"

namespace kiv_vss::utils
{
    bool Try_Event(double probability)
    {
        // Generate a random number <0; 1>.
        std::random_device rand_dev{};
        std::uniform_real_distribution<> uniform_dist{};

        return uniform_dist(rand_dev) <= probability;
    }

    size_t Pick_Event(std::vector<double> probabilities)
    {
        std::sort(probabilities.begin(), probabilities.end());
        for (size_t i = 1; i < probabilities.size(); ++i)
        {
            probabilities[i] += probabilities[i - 1];
        }

        // Make sure the probabilities add up to 100%.
        static constexpr double EPSILON = 0.0001;
        if (std::abs(probabilities.back() - 1.0) > EPSILON)
        {
            spdlog::error("ERROR: Sum of all probabilities must add up to 1.0, not {}. Returning 0 as the default value.", probabilities.back());
            return 0;
        }

        // Generate a random number <0; 1>.
        std::random_device rand_dev{};
        std::uniform_real_distribution<> uniform_dist{};
        const double value = uniform_dist(rand_dev);

        // Binary search the interval the random number falls into.
        auto it = std::upper_bound(probabilities.begin(), probabilities.end(), value);

        // Make sure we do not overshoot.
        if (it == probabilities.end())
        {
            std::advance(it, -1);
        }

        // Return the index of the event.
        return static_cast<size_t>(it - probabilities.begin());
    }
}

// EOF