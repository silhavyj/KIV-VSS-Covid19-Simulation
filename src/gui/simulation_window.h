#pragma once

#include "gui_window.h"

namespace kiv_vss::gui
{
    class CSimulation_Window : public GUI_Window
    {
    public:
        explicit CSimulation_Window(CSimulation* simulation, const bool* display_popular_locations);
        ~CSimulation_Window() override = default;

        void Render() override;

    private:
        static constexpr float Person_Circle_Size = 4.0f;
        static constexpr float Popular_Location_Circle_Size = 2 * Person_Circle_Size;
        static constexpr auto Popular_Location_Color = ImVec4(1.0f, 1.0f, 0.0f, 0.75f);

    private:
        void Draw_Person(const CPerson& person, ImDrawList* draw_list);
        void Draw_Popular_Location(const CLocation& location, ImDrawList* draw_list);
        [[nodiscard]] static std::pair<ImVec2, ImVec2> Get_Windows_Boundaries();
        [[nodiscard]] static ImVec4 Get_Person_Color(const CPerson& person);
        void Add_Popular_Location();

    private:
        const bool* m_display_popular_locations;
    };
}