#pragma once

#include "gui_window.h"

namespace kiv_vss::gui
{
    class CPlots_Window : public GUI_Window
    {
    public:
        explicit CPlots_Window(CSimulation* simulation);
        ~CPlots_Window() = default;

        void Render() override;
    };
}