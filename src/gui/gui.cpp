#include <vector>

#include "implot.h"
#include "gui.h"

#include "settings_window.h"
#include "simulation_window.h"
#include "plots_window.h"
#include "statistics_window.h"

namespace kiv_vss::gui
{
    // Simulation (backend logic)
    static CSimulation* s_simulation = new CSimulation;

    static bool s_play{false};
    static bool s_stop_simulation_when_nobody_infected{true};
    static bool s_simulation_over{false};
    static bool s_simulation_running{false};
    static bool s_display_popular_locations{true};
    static bool s_general_settings_changed{false};

    // Create individual windows that make up the GUI.
    static CSettings_Window s_settings_window(s_simulation, &s_simulation_running, &s_general_settings_changed);
    static CSimulation_Window s_simulation_window(s_simulation, &s_display_popular_locations, &s_simulation_running);
    static CPlots_Window s_plots_window(s_simulation);
    static CStatistics_Window s_statistics_window(s_simulation);

    // Store the windows into a polymorphic container.
    static const std::vector<GUI_Window *> s_windows = {
        &s_settings_window,
        &s_simulation_window,
        &s_plots_window,
        &s_statistics_window
    };

    // Function prototypes.
    static void Render_Control_Window();
    static void Update_Simulation();
    static void Render_Popular_Places_Table();

    void Render_GUI()
    {
        Render_Control_Window();
        Update_Simulation();

        // Render individual windows.
        std::for_each(s_windows.begin(), s_windows.end(), [&](auto& window) { window->Render(); });

        // Just for test/development purposes
        //
        // ImPlot::ShowDemoWindow();
        ImGui::ShowDemoWindow();
    }

    inline static void Update_Simulation()
    {
        // Update the simulation unless it is over or the user pressed the pause button.
        if (s_play && !s_simulation_over)
        {
            s_simulation->Update();

            // Check whether the simulation is over.
            s_simulation_running = true;
            if (s_stop_simulation_when_nobody_infected)
            {
                s_simulation_over = s_simulation->Is_Simulation_Over();
                s_simulation_running = !s_simulation_over;
            }
        }
    }

    inline static void Render_Control_Window()
    {
        // Start the windows
        ImGui::Begin("Control");

        // Play / Pause button
        if (!s_general_settings_changed && ImGui::Button("Play / Pause"))
        {
            s_play = !s_play;
        }

        // Put the next element on the same line.
        ImGui::SameLine();

        // Reset button
        if (ImGui::Button("Reset"))
        {
            // Create a new instance of the backend logic of the simulation.
            delete s_simulation;
            s_simulation = new CSimulation;

            // Set the new simulation to all windows.
            std::for_each(s_windows.begin(), s_windows.end(), [&](auto& window) { window->Set_Simulation(s_simulation); });

            s_play = false;
            s_simulation_over = false;
            s_simulation_running = false;
            s_general_settings_changed = false;
        }

        ImGui::Separator();

        ImGui::Checkbox("Stop the simulation if there are\nno infected people", &s_stop_simulation_when_nobody_infected);
        ImGui::Checkbox("Display popular locations", &s_display_popular_locations);

        ImGui::Separator();

        if (!s_simulation_running)
        {
            Render_Popular_Places_Table();
        }

        // End the window.
        ImGui::End();
    }

    static void Render_Popular_Places_Table()
    {
        const auto Get_Input_Name = [](int index, const char* desc) {
            std::string name = std::to_string(index) + std::string(desc);
            return name.c_str();
        };

        // Retrieve all popular locations.
        auto& popular_locations = s_simulation->Get_Popular_Locations();

        // Flags (design) of the table
        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        if (ImGui::BeginTable("Popular locations", 3, flags))
        {
            // Headings
            ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableHeadersRow();

            for (std::size_t i = 0; i < popular_locations.size(); ++i)
            {
                // Start a new table row.
                ImGui::TableNextRow();

                // First column (index).
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%ld", i);

                // First column (X position).
                ImGui::TableSetColumnIndex(1);
                ImGui::InputDouble(Get_Input_Name(i, "x"), popular_locations[i].Get_X(), 10, 1, "%.2f");

                // First column (Y position).
                ImGui::TableSetColumnIndex(2);
                ImGui::InputDouble(Get_Input_Name(i, "y"), popular_locations[i].Get_Y(), 10, 1, "%.2f");
            }

            ImGui::EndTable();
        }
    }
}

// EOF