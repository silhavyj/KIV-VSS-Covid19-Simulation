#include <vector>

#include "gui.h"
#include "settings.h"
#include "simulation.h"

namespace kiv_vss::gui
{
    static CSimulation* s_simulation = new CSimulation;

    static CSettings_Window s_settings_window;
    static CSimulation_Window s_simulation_window(s_simulation);

    static std::vector<IGUI_Window *> s_windows = {
        &s_settings_window,
        &s_simulation_window
    };

    void Render_GUI()
    {
        s_simulation->Update();
        std::for_each(s_windows.begin(), s_windows.end(), [&](auto& window) { window->Render(); });
    }
}