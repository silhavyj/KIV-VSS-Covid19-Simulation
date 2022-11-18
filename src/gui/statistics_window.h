#pragma once

#include "gui_window.h"

namespace kiv_vss::gui
{
    class CStatistics_Window : public GUI_Window
    {
    public:
        CStatistics_Window(const CSimulation* simulation);
        ~CStatistics_Window() = default;

        void Render() override;

    private:
        double Get_Number_Of_People_Percentage(const std::vector<float>& data) const;
    };
}