#include <mutex>
#include <atomic>
#include <thread>

#include "imgui.h"
#include "implot.h"
#include "spdlog/spdlog.h"

#include "gui.h"
#include "../simulation/simulator.h"
#include "../simulation/singleton.h"

namespace kiv_vss::gui
{
    static void Draw_Simulation();
    static void Draw_Control_Panel();
    static void Draw_Settings_Panel();
    static void Draw_Charts();
    static void Draw_Statistics();
    static void Draw_Person(CPerson& person, ImDrawList* draw_list);
    static void Draw_Popular_Location(const CLocation& location, ImDrawList* draw_list);
    static std::pair<ImVec2, ImVec2> Get_Windows_Boundaries();
    static ImVec4 Get_Person_Color(const CPerson& person);
    static void Run_Simulation();

    static CSimulator* s_simulator = new CSimulator;
    const static auto s_config = Singleton<TConfig>::Get_Instance();
    static bool s_display_popular_locations = true;
    static bool s_stop_sim_whe_no_people_infected = true;
    static bool s_sim_running = false;
    static double s_number_of_infections_per_person = 0;
    const static uint32_t Sleep_For_Ms = 20;
    static size_t s_infected_peek = 0;
    static std::vector<float> s_number_of_infected_people;
    static std::vector<float> s_number_of_fatalities;
    static std::vector<float> s_number_of_immune_people;
    static std::vector<float> s_number_of_vulnerable_people;
    static std::vector<float> s_days;

    static ImVec4 s_vulnerable_color = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);
    static ImVec4 s_infected_color   = ImVec4(0.0f, 1.0f, 0.0f, 0.5f);
    static ImVec4 s_immune_color     = ImVec4(0.0f, 0.0f, 1.0f, 0.5f);
    static ImVec4 s_dead_color       = ImVec4(1.0f, 0.0f, 0.0f, 0.5f);

    static ImVec4 s_popular_location_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    static std::atomic<bool> s_stop_simulation{true};
    static std::mutex s_mtx;

    static void Run_Simulation()
    {
        static size_t counter = 0;
        static size_t days = 0;

        while (!s_stop_simulation)
        {
            s_mtx.lock();
            s_simulator->Update();
            s_mtx.unlock();
            if (counter % 24 == 0)
            {
                s_number_of_fatalities.push_back(s_simulator->Get_Number_Of_Fatalities());
                s_number_of_infected_people.push_back(s_simulator->Get_Number_Of_Infected_People());
                s_number_of_immune_people.push_back(s_simulator->Get_Number_Of_Immune_People());
                s_number_of_vulnerable_people.push_back(s_simulator->Get_Number_Of_Vulnerable_People());
                s_days.push_back(days);
                s_number_of_infections_per_person = s_simulator->Get_Number_Of_Infections_Per_Person();
                ++days;
            }
            if (s_stop_sim_whe_no_people_infected && 0 == s_simulator->Get_Number_Of_Infected_People())
            {
                days = 0;
                counter = 0;
                break;
            }
            ++counter;
            std::this_thread::sleep_for(std::chrono::milliseconds(Sleep_For_Ms));
        }
        if (!s_sim_running)
        {
            days = 0;
            counter = 0;
        }
    }

    void Render_GUI()
    {
        Draw_Simulation();
        Draw_Control_Panel();
        Draw_Settings_Panel();
        Draw_Charts();
        Draw_Statistics();
        //ImGui::ShowDemoWindow();
    }

    static void Draw_Statistics()
    {
        ImGui::Begin("Statistics");
        size_t currently_infected = 0;
        size_t diseased_people = 0;
        if (!s_days.empty())
        {
            currently_infected = s_number_of_infected_people.back();
            s_infected_peek = std::max(s_infected_peek, currently_infected);
            diseased_people = s_number_of_fatalities.back();
        }
        ImGui::Text("Currently infected = %d (%.2f%)", currently_infected, 100.0 * ((double)currently_infected / s_config->Number_Of_People));
        ImGui::Text("Diseased people = %d (%.2f%)", diseased_people, 100.0 * ((double)diseased_people / s_config->Number_Of_People));
        ImGui::Text("Infected peek = %d (%.2f%)", s_infected_peek, 100.0 * ((double)s_infected_peek / s_config->Number_Of_People));
        ImGui::Text("Average number of infections per person = %.2f", s_number_of_infections_per_person);
        ImGui::End();
    }

    static void Draw_Charts()
    {
        ImGui::Begin("Charts");
        if (ImPlot::BeginPlot("Statistics"))
        {
            ImPlot::PlotLine("Number of infected people", s_days.data(), s_number_of_infected_people.data(), s_days.size());
            ImPlot::PlotLine("Number of fatalities", s_days.data(), s_number_of_fatalities.data(), s_days.size());
            //ImPlot::PlotLine("Number of vulnerable people", s_days.data(), s_number_of_vulnerable_people.data(), s_days.size());
            //ImPlot::PlotLine("Number of immune people", s_days.data(), s_number_of_immune_people.data(), s_days.size());
            ImPlot::EndPlot();
        }
        ImGui::End();
    }

    static void Draw_Settings_Panel()
    {
        ImGui::Begin("Settings");

        ImGui::SliderInt("Number of popular places", reinterpret_cast<int *>(&s_config->Number_Of_Popular_Locations), 0, 20);
        ImGui::Checkbox("Display popular places", &s_display_popular_locations);
        // ImGui::SliderInt("Number of people", reinterpret_cast<int *>(&s_config->Number_Of_People), 1, 5000);
        ImGui::Separator();

        ImGui::SliderInt("Number of initially infected people", reinterpret_cast<int *>(&s_config->Number_Of_Initially_Infected_People), 1, 20);
        ImGui::SliderFloat("% of self isolating people", &s_config->Ratio_Of_People_In_Self_Isolation, 0.0f, 1.0f);
        ImGui::SliderFloat("Disease transmission distance", &s_config->Disease_Transmission_Distance, 0.0f, 10.0f);
        ImGui::SliderFloat("Disease transmission probability (at home)", &s_config->Disease_Transmission_Probability_At_Home, 0.0f, 1.0f, ".%3f");
        ImGui::SliderFloat("Disease transmission probability (outside)", &s_config->Disease_Transmission_Probability_On_Move, 0.0f, 1.0f, ".%3f");
        ImGui::SliderFloat("Probability of going to self isolate when infected", &s_config->Self_Isolating_When_Infected, 0.0f, 1.0f, ".%3f");
        ImGui::SliderFloat("Death probability", &s_config->Death_Probability, 0.0f, 1.0f, ".%3f");
        ImGui::Separator();
        ImGui::SliderInt("Average infection period", reinterpret_cast<int *>(&s_config->Average_Infection_Period), 24, 960);
        ImGui::SliderInt("Average immunity period", reinterpret_cast<int *>(&s_config->Average_Immunity_Period), 24, 2160);
        ImGui::Separator();

        ImGui::End();
    }

    static void Draw_Simulation()
    {
        ImGui::Begin("Simulation");

        const auto& people = s_simulator->Get_People();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        std::for_each(people.begin(), people.end(), [&](auto person) { Draw_Person(person, draw_list); });

        if (s_display_popular_locations)
        {
            const auto& popular_locations = s_simulator->Get_Popular_Locations();
            std::for_each(popular_locations.begin(), popular_locations.end(), [&](const auto location) {
                Draw_Popular_Location(location, draw_list);
            });
        }
        ImGui::End();
    }

    static void Draw_Popular_Location(const CLocation& location, ImDrawList* draw_list)
    {
        const auto [y, x] = location.Get_Coordinates();
        auto [v_min, v_max ] = Get_Windows_Boundaries();

        static constexpr float Popular_Location_Circle_Size = 8.0;
        v_min.y += (((v_max.y - v_min.y) * y) / s_config->World_Size) - Popular_Location_Circle_Size / 2.0;
        v_min.x += (((v_max.x - v_min.x) * x) / s_config->World_Size) - Popular_Location_Circle_Size / 2.0;

        draw_list->AddCircleFilled(v_min, Popular_Location_Circle_Size, ImGui::GetColorU32(s_popular_location_color));
    }

    static void Draw_Person(CPerson& person, ImDrawList* draw_list)
    {
        const auto [y, x] = person.Get_Current_Location().Get_Coordinates();
        auto [v_min, v_max ] = Get_Windows_Boundaries();

        static constexpr float Person_Circle_Size = 4.0;
        v_min.y += (((v_max.y - v_min.y) * y) / s_config->World_Size) - Person_Circle_Size / 2.0;
        v_min.x += (((v_max.x - v_min.x) * x) / s_config->World_Size) - Person_Circle_Size / 2.0;

        draw_list->AddCircleFilled(v_min, Person_Circle_Size, ImGui::GetColorU32(Get_Person_Color(person)));
    }

    static std::pair<ImVec2, ImVec2> Get_Windows_Boundaries()
    {
        ImVec2 v_max = ImGui::GetWindowContentRegionMax();
        ImVec2 v_min = ImGui::GetWindowContentRegionMin();

        v_min.x += ImGui::GetWindowPos().x;
        v_min.y += ImGui::GetWindowPos().y;
        v_max.x += ImGui::GetWindowPos().x;
        v_max.y += ImGui::GetWindowPos().y;

        return { v_min, v_max };
    }

    static ImVec4 Get_Person_Color(const CPerson& person)
    {
        const auto state = person.Get_Infection_State();

        switch (state)
        {
            case CPerson::NInfection_State::Immune:
                return s_immune_color;

            case CPerson::NInfection_State::Infected:
                return s_infected_color;

            case CPerson::NInfection_State::Vulnerable:
                return s_vulnerable_color;

            case CPerson::NInfection_State::Dead:
                return s_dead_color;

            default:
                spdlog::error("Person's color could not be determined. Returning {} as the default color.");
                return {};
        }
    }

    static void Draw_Control_Panel()
    {
        ImGui::Begin("Control panel");

        if (ImGui::Button("Play"))
        {
            if (!s_sim_running)
            {
                std::thread t(&Run_Simulation);
                t.detach();
                s_stop_simulation = false;
                s_sim_running = true;
            }
        }
        if (ImGui::Button("Pause/Resume"))
        {
            if (s_sim_running)
            {
                if (s_stop_simulation)
                {
                    std::thread t(&Run_Simulation);
                    t.detach();
                    s_stop_simulation = false;
                }
                else
                {
                    s_stop_simulation = true;
                }
            }
        }
        if (ImGui::Button("Reset"))
        {
            s_mtx.lock();
            s_stop_simulation = true;
            delete s_simulator;
            s_simulator = new CSimulator;
            s_sim_running = false;
            s_number_of_fatalities.clear();
            s_number_of_infected_people.clear();
            s_days.clear();
            s_infected_peek = 0;
            s_number_of_infections_per_person = 0;
            s_mtx.unlock();
        }
        ImGui::Checkbox("Stop the simulation there are\nno infected people", &s_stop_sim_whe_no_people_infected);
        ImGui::End();
    }
}