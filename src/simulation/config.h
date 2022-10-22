#pragma once

namespace kiv_vss
{
    struct Config
    {
        double World_Size = 5000;
        size_t Number_Of_Popular_Places = 10;
        size_t Number_Of_People = 1000;
        float Ratio_Of_People_In_Self_Isolation = 0.5f;
        double Downtown_Location = World_Size / 2.0;
        double Downtown_Variance = World_Size * 0.15;
        size_t Number_Of_Initially_Infected_People = 5;
        double Disease_Transmission_Distance = 5;
        double Disease_Transmission_Probability = 0.70;

        // mobility
        double Average_Person_Mobility_Speed = 4;
        double Person_Mobility_Speed_Variance = 1.5;
        double Max_Hours_Spent_At_Location = 8;

        double Go_To_Popular_Location_Probability_1 = 0.4;
        double Go_Home_Probability_1 = 0.4;
        double Go_To_Random_Location_Probability_1 = 0.2;

        double Go_To_Popular_Location_Probability_2 = 0.80;
        double Go_To_Random_Location_Probability_2 = 0.20;

        size_t Average_Infection_Period = 24 * 14;
        size_t Infection_Period_Variance = 24 * 5;

        size_t Covid_Immunity_Period = 24 * 60;
        size_t Covid_Immunity_Variance = 24 * 10;

        double Death_Probability = 0.15;
    };
}