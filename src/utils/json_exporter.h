#pragma once

#include "../simulation/simulation.h"

namespace kiv_vss::utils
{
    /// Experts measured statistical values from the simulation into a JSON file.
    /// \param filename Name of the output file
    /// \param statistics Statistics to be saved into the file
    /// \return True, if the values were saved successfully, false otherwise.
    bool Export_Statistics(const char* filename, const CSimulation::TStatistics& statistics);
}

// EOF