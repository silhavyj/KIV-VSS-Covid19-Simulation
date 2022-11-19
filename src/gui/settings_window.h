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

    private:
        void Render_General_Settings() const;
        void Render_Disease_Settings() const;
    };
}