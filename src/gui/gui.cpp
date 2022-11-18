#include <vector>

#include "gui.h"
#include "settings.h"
#include "simulation.h"

namespace kiv_vss::gui
{
    static CSimulation* s_simulation = new CSimulation;
    static bool s_play{false};

    static CSettings_Window s_settings_window;
    static CSimulation_Window s_simulation_window(s_simulation);

    static std::vector<IGUI_Window *> s_windows = {
        &s_settings_window,
        &s_simulation_window
    };

    static void Render_Control_Window();
    static void Update_Simulation();

    void Render_GUI()
    {
        Render_Control_Window();
        Update_Simulation();
        std::for_each(s_windows.begin(), s_windows.end(), [&](auto& window) { window->Render(); });
    }

    inline static void Update_Simulation()
    {
        if (s_play)
        {
            s_simulation->Update();
        }
    }

    inline static void Render_Control_Window()
    {
        ImGui::Begin("Control");
        if (ImGui::Button("Reset"))
        {
            delete s_simulation;
            s_simulation = new CSimulation;
            s_simulation_window.Set_Simulation(s_simulation);
            s_play = false;
        }

        if (ImGui::Button("Play / Pause"))
        {
            s_play = !s_play;
        }
        ImGui::End();
    }
}