#pragma once

#include "gui_window.h"
#include "../simulation/config.h"

namespace kiv_vss::gui
{
    class CSettings_Window : public IGUI_Window
    {
    public:
        CSettings_Window();
        ~CSettings_Window() = default;

        void Render() override;

    private:
        TConfig* m_config;
    };
}