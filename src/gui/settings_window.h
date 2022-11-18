#pragma once

#include "gui_window.h"

namespace kiv_vss::gui
{
    class CSettings_Window : public GUI_Window
    {
    public:
        CSettings_Window(const CSimulation* simulation);
        ~CSettings_Window() = default;

        void Render() override;
    };
}