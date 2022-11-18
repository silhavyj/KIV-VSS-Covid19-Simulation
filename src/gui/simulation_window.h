#pragma once

#include "gui_window.h"

namespace kiv_vss::gui
{
    class CSimulation_Window : public GUI_Window
    {
    public:
        CSimulation_Window(const CSimulation* simulation);
        ~CSimulation_Window() = default;

        void Render() override;

    private:
        static constexpr float Person_Circle_Size = 4.0f;

    private:
        void Draw_Person(const CPerson& person, ImDrawList* draw_list);
        [[nodiscard]] static std::pair<ImVec2, ImVec2> Get_Windows_Boundaries();
        [[nodiscard]] ImVec4 Get_Person_Color(const CPerson& person);
    };
}