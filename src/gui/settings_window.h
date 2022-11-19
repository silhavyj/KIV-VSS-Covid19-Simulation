#pragma once

#include "gui_window.h"

namespace kiv_vss::gui
{
    class CSettings_Window : public GUI_Window
    {
    public:
        explicit CSettings_Window(CSimulation* simulation);
        ~CSettings_Window() override = default;

        void Render() override;

    private:
        void Render_General_Settings() const;
        void Render_Disease_Settings() const;
        void Render_Mobility_Settings() const;
    };
}