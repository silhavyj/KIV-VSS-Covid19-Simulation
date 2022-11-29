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
        // Helper struct to hold the probability of an event and its index
        struct TSample
        {
            double probability;
            std::size_t index;

            bool operator<(const TSample& other) const
            {
                return probability < other.probability;
            }
        };

        // Create samples out of the probabilities.
        std::vector<TSample> samples;
        for (std::size_t i = 0; i < probabilities.size(); ++i)
        {
            samples.push_back({probabilities[i], i});
        }
        
        std::sort(samples.begin(), samples.end());
        for (size_t i = 1; i < samples.size(); ++i)
        {
            samples[i].probability += samples[i - 1].probability;
        }

        // Make sure the probabilities add up to 100%.
        static constexpr double EPSILON = 0.0001;
        if (std::abs(samples.back().probability - 1.0) > EPSILON)
        {
            spdlog::error("ERROR: Sum of all probabilities must add up to 1.0, not {}. Returning 0 as the default value.", samples.back().probability);
            return 0;
        }

        // Generate a random number <0; 1>.
        std::random_device rand_dev{};
        std::uniform_real_distribution<> uniform_dist{};
        const double value = uniform_dist(rand_dev);

        // Binary search the interval the random number falls into.
        auto it = std::upper_bound(samples.begin(), samples.end(), value, [&](double probability, const TSample& sample) {
            return probability < sample.probability;
        });

        // Make sure we do not overshoot.
        if (it == samples.end())
        {
            std::advance(it, -1);
        }

        // Return the index of the event.
        return it->index;
    }
}

// EOF