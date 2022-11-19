#include <vector>
#include <algorithm>

#include "implot.h"
#include "gui.h"

#include "settings_window.h"
#include "simulation_window.h"
#include "plots_window.h"
#include "statistics_window.h"

namespace kiv_vss::gui
{
    static CSimulation* s_simulation = new CSimulation;
    static bool s_play{false};
    static bool s_stop_simulation_when_nobody_infected{true};
    static bool s_simulation_over{false};
    static bool s_display_popular_locations{true};

    static CSettings_Window s_settings_window(s_simulation);
    static CSimulation_Window s_simulation_window(s_simulation, &s_display_popular_locations);
    static CPlots_Window s_plots_window(s_simulation);
    static CStatistics_Window s_statistics_window(s_simulation);

    static std::vector<GUI_Window *> s_windows = {
        &s_settings_window,
        &s_simulation_window,
        &s_plots_window,
        &s_statistics_window
    };

    static void Render_Control_Window();
    static void Update_Simulation();

    void Render_GUI()
    {
        Render_Control_Window();
        Update_Simulation();
        std::for_each(s_windows.begin(), s_windows.end(), [&](auto& window) { window->Render(); });

        // TODO just a test
        // ImPlot::ShowDemoWindow();
        // ImGui::ShowDemoWindow();
    }

    inline static void Update_Simulation()
    {
        if (s_play && !(s_stop_simulation_when_nobody_infected && s_simulation_over))
        {
            s_simulation->Update();
            s_simulation_over = s_simulation->Is_Simulation_Over();
        }
    }

    inline static void Render_Control_Window()
    {
        ImGui::Begin("Control");

        if (ImGui::Button("Play / Pause"))
        {
            s_play = !s_play;
        }

        ImGui::SameLine();

        if (ImGui::Button("Reset"))
        {
            delete s_simulation;
            s_simulation = new CSimulation;
            std::for_each(s_windows.begin(), s_windows.end(), [&](auto& window) { window->Set_Simulation(s_simulation); });

            s_play = false;
            s_simulation_over = false;
        }

        ImGui::Checkbox("Stop the simulation if there are\nno infected people", &s_stop_simulation_when_nobody_infected);
        ImGui::Checkbox("Display popular locations", &s_display_popular_locations);

        ImGui::End();
    }
}