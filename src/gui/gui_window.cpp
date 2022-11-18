#include "gui_window.h"
#include "../simulation/singleton.h"

namespace kiv_vss::gui
{
    GUI_Window::GUI_Window(const CSimulation* simulation)
        : m_simulation{simulation},
          m_config{Singleton<TConfig>::Get_Instance()}
    {

    }

    void GUI_Window::Set_Simulation(const CSimulation* simulation)
    {
        m_simulation = simulation;
    }
}