#pragma once

#include "../simulation/simulation.h"

namespace kiv_vss::utils
{
    bool Export_Statistics(const char* filename, const CSimulation::TStatistics& statistics);
}