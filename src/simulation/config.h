#pragma once

#include <cstddef>

namespace kiv_vss
{
    struct TConfig
    {
        double World_Size = 5000;
        size_t Number_Of_Popular_Locations = 10;
        size_t Number_Of_People = 1000;
        float Ratio_Of_People_In_Self_Isolation = 0.5f;

        double Downtown_Center_Location = World_Size / 2.0;
        double Downtown_Variance = World_Size * 0.15;

        size_t Number_Of_Initially_Infected_People = 5;
        float Disease_Transmission_Distance = 5;
        float Disease_Transmission_Probability_On_Move = 0.65;
        float Disease_Transmission_Probability_At_Home = 0.5;
        float Self_Isolating_When_Infected = 0.4;
        float Death_Probability = 0.15;

        double Average_Person_Mobility_Speed = 10;
        double Variance_Person_Mobility_Speed = 1;
        double Max_Hours_Spent_At_Location = 8;

        double Go_To_Popular_Location_Probability_1 = 0.4;
        double Go_To_Random_Location_Probability_1 = 0.2;
        double Go_Home_Probability_1 = 0.4;

        size_t Average_Infection_Period = 24 * 14;
        size_t Average_Immunity_Period = 24 * 60;

        double Go_To_Popular_Location_Probability_2 = 0.80;
        double Go_To_Random_Location_Probability_2 = 0.20;

        double Go_To_Popular_Location_Probability_3 = 0.12;
        double Go_To_Random_Location_Probability_3 = 0.12;
        double Go_Home_Probability_3 = 0.76;

        double Go_To_Popular_Location_Probability_4 = 0.2;
        double Go_To_Random_Location_Probability_4 = 0.04;
        double Go_Home_Probability_4 = 0.76;
    };
}