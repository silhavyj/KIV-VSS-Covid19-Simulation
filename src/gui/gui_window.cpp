#include "gui_window.h"
#include "../utils/singleton.h"

namespace kiv_vss::gui
{
    GUI_Window::GUI_Window(CSimulation* simulation)
        : m_simulation{simulation},
          m_config{Singleton<TConfig>::Get_Instance()}
    {

    }

    void GUI_Window::Set_Simulation(CSimulation* simulation)
    {
        m_simulation = simulation;
    }
}

// EOF