#pragma once

#include <vector>

#include "config.h"
#include "person.h"
#include "mobility_model.h"
#include "infection_model.h"

namespace kiv_vss
{
    class CSimulation
    {
    public:
        struct TStatistics_Record
        {
            float number_of_fatalities{0};
            float number_of_infected_people{0};
            float number_of_immune_people{0};
            float number_of_susceptible_people{0};
            double number_of_infections_per_person{0.0};
        };

        struct TStatistics
        {
            std::vector<float> number_of_fatalities;
            std::vector<float> number_of_infected_people;
            std::vector<float> number_of_immune_people;
            std::vector<float> number_of_susceptible_people;
            std::vector<float> number_of_contractions_per_person;
            std::vector<float> time;
        };

    public:
        CSimulation();
        ~CSimulation() = default;

        void Update();
        void Add_Popular_Location(const CLocation& location);
        [[nodiscard]] const TStatistics& Get_Statistics() const;
        [[nodiscard]] const std::vector<CPerson>& Get_People() const;
        [[nodiscard]] const std::vector<CLocation>& Get_Popular_Locations() const;
        [[nodiscard]] bool Is_System_Saturated() const;
        [[nodiscard]] bool Is_Simulation_Over() const;
        [[nodiscard]] std::size_t Get_Maximum_Number_Of_Infected_People() const;

    private:
        void Update_Statistics(const TStatistics_Record& record);
        void Update_Statistics_Record(TStatistics_Record& record, const CPerson& person, std::size_t infected_count);
        void Generate_Population();
        void Move_People_Around();
        void Spread_Virus();
        [[nodiscard]] bool Try_To_Infect(const CInfection_Model& infected_person_manager, const CInfection_Model& vulnerable_person_manager) const;

    private:
        std::vector<CPerson> m_people;
        std::vector<CLocation> m_popular_locations;
        const TConfig* m_config;
        std::vector<CMobility_Model> m_mobility_managers;
        std::vector<CInfection_Model> m_infection_managers;
        TStatistics m_statistics;
        std::size_t m_time;
        bool m_system_saturated;
        std::size_t m_max_number_of_infected_people;
    };
}