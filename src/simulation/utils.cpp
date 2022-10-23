#include <cassert>

#include "utils.h"

namespace kiv_vss::utils
{
    bool Try_Event(double probability)
    {
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
        // TODO print out an error message
        assert((probabilities.back() - 1.0) < 0.0001);

        std::random_device rand_dev{};
        std::uniform_real_distribution<> uniform_dist{};

        double value = uniform_dist(rand_dev);
        auto it = std::upper_bound(probabilities.begin(), probabilities.end(), value);

        if (it == probabilities.end())
        {
            std::advance(it, -1);
        }
        return static_cast<size_t>(it - probabilities.begin());
    }
}