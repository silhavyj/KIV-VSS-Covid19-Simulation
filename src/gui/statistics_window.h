#pragma once

#include "gui_window.h"

namespace kiv_vss::gui
{
    class CStatistics_Window : public GUI_Window
    {
    public:
        explicit CStatistics_Window(CSimulation* simulation);
        ~CStatistics_Window() override = default;

        void Render() override;

    private:
        [[nodiscard]] double Get_Number_Of_People_Percentage(const std::vector<float>& data) const noexcept;
        [[nodiscard]] double Get_Percentage_People(double value) const noexcept;
    };
}

// EOF