#include <fstream>

#include "json_exporter.h"

#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"

namespace kiv_vss::utils
{
    bool Export_Statistics(const char* filename, const CSimulation::TStatistics& statistics)
    {
        nlohmann::json::array_t data;

        // Construct a JSON object (array).
        for (std::size_t i = 0; i < statistics.time.size(); ++i)
        {
            const nlohmann::json record = {
                { "time"       , statistics.time[i]                         },
                { "infected"   , statistics.number_of_infected_people[i]    },
                { "immune"     , statistics.number_of_immune_people[i]      },
                { "dead"       , statistics.number_of_fatalities[i]         },
                { "susceptible", statistics.number_of_susceptible_people[i] }
            };
            data.push_back(record);
        }

        // Store the data into the file.
        std::fstream file(filename, std::ios::out);
        if (file)
        {
            file << data;
            spdlog::info("Data was saved successfully to '{}'", filename);
            return true;
        }

        spdlog::error("Failed to open file '{}'", filename);
        return false;
    }
}

// EOF