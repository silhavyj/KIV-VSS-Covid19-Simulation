#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "location.h"
#include "person.h"
#include "config.h"
#include "mobility_manager.h"
#include "infection_manager.h"

namespace kiv_vss
{
    class CSimulator
    {
    public:
        CSimulator();
        ~CSimulator() = default;

        void Update();
        [[nodiscard]] const std::vector<CPerson>& Get_People() const;
        [[nodiscard]] const std::vector<CLocation>& Get_Popular_Locations() const;
        [[nodiscard]] size_t Get_Number_Of_Infected_People() const;
        [[nodiscard]] size_t Get_Number_Of_Fatalities() const;
        [[nodiscard]] size_t Get_Number_Of_Immune_People() const;
        [[nodiscard]] size_t Get_Number_Of_Vulnerable_People() const;
        [[nodiscard]] double Get_Number_Of_Infections_Per_Person() const;

    private:
        void Generate_Popular_Locations();
        void Generate_People();
        void Move_People_Around();
        void Spread_Virus();
        [[nodiscard]] bool Infects(const CInfection_Manger* infected_person_manager, const CInfection_Manger* vulnerable_person_manager) const;

        [[nodiscard]] CLocation Generate_Random_Downtown_Location() const;
        [[nodiscard]] CLocation Generate_Random_Location() const;

    private:
        std::vector<CPerson> m_people;
        std::vector<CLocation> m_popular_locations;
        const TConfig* m_config;
        std::vector<CMobility_Manager> m_mobility_managers;
        std::vector<CInfection_Manger> m_infection_managers;
        std::unordered_set<CInfection_Manger*> m_infected_people_mngs;
        std::unordered_set<CInfection_Manger*> m_vulnerable_people_mngs;
        std::unordered_map<CPerson*, CInfection_Manger*> m_mobility_managers_map;
        std::unordered_set<CPerson*> m_fatalities;
        double m_number_of_infections_per_person;
        size_t m_number_of_vulnerable_people;
        size_t m_number_of_immune_people;
    };
}