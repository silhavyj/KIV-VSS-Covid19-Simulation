#pragma once

#include <random>

namespace kiv_vss::utils
{
    template<typename Distribution, typename ...Args>
    auto Random(const Args... args)
    {
        static std::random_device rand_dev{};
        Distribution distribution(args...);
        return distribution(rand_dev);
    }

    bool Try_Event(double probability);

    size_t Pick_Event(std::vector<double> probabilities);
}

// EOF