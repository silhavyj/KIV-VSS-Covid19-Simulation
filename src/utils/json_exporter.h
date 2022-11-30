#pragma once

#include "../simulation/simulation.h"

namespace kiv_vss::utils
{
    void Export_Statistics(const char* filename, const CSimulation::TStatistics& statistics);
}