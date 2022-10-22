#pragma once

#include <vector>
#include <cstddef>

#include "location.h"
#include "person.h"
#include "person_mobility.h"

namespace kiv_vss
{
    class CSimulator
    {
    public:
        CSimulator();
        ~CSimulator() = default;

        [[nodiscard]] const std::vector<CLocation>& Get_Popular_Locations() const;
        [[nodiscard]] const std::vector<CPerson>& Get_People() const;
        [[nodiscard]] size_t Get_Number_Of_Infected_People() const;
        [[nodiscard]] size_t Get_Number_Of_Fatalities() const;
        [[nodiscard]] size_t Get_Number_Immune_People() const;

        void Update();

    private:
        void Generate_Popular_Locations(const size_t count);
        void Generate_People(const size_t count);
        void Move_People_Around();
        void Spread_Virus();

        static CLocation Generate_Random_Downtown_Location();
        static CLocation Generate_Random_Location();
        static bool Infects(CPerson& person_i, CPerson& person_j);

    private:
        std::vector<CPerson> m_people;
        std::vector<CPerson_Mobility> m_person_mobility;
        std::vector<CLocation> m_popular_locations;
        size_t m_number_of_infected_people;
        size_t m_number_of_fatalities;
        size_t m_number_of_immune_people;
    };
}