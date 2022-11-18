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
        CSimulation();
        ~CSimulation() = default;

        void Update();
        void Add_Popular_Location(const CLocation& location);
        [[nodiscard]] const std::vector<CPerson>& Get_People() const;
        [[nodiscard]] const std::vector<CLocation>& Get_Popular_Locations() const;

    private:
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
    };
}