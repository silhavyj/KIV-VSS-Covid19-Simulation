#pragma once

#include "imgui.h"

#include "../simulation/config.h"
#include "../simulation/simulation.h"

namespace kiv_vss::gui
{
    class GUI_Window
    {
    public:
        GUI_Window(const CSimulation* simulation);
        virtual ~GUI_Window() = default;

        void Set_Simulation(const CSimulation* simulation);
        virtual void Render() = 0;

    protected:
        const CSimulation* m_simulation;
        TConfig* m_config;
    };
}