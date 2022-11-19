#pragma once

#include "imgui.h"

#include "../simulation/config.h"
#include "../simulation/simulation.h"

namespace kiv_vss::gui
{
    class GUI_Window
    {
    public:
        explicit GUI_Window(CSimulation* simulation);
        virtual ~GUI_Window() = default;

        void Set_Simulation(CSimulation* simulation);
        virtual void Render() = 0;

    protected:
        CSimulation* m_simulation;
        TConfig* m_config;
    };
}