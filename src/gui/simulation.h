#pragma once

#include <mutex>

#include "imgui.h"

#include "gui_window.h"
#include "../simulation/config.h"
#include "../simulation/simulation.h"

namespace kiv_vss::gui
{
    class CSimulation_Window : public IGUI_Window
    {
    public:
        CSimulation_Window(const CSimulation* simulation);
        ~CSimulation_Window() = default;

        void Set_Simulation(const CSimulation* simulation);
        void Render() override;

    private:
        static constexpr float Person_Circle_Size = 4.0f;

    private:
        void Draw_Person(const CPerson& person, ImDrawList* draw_list);
        [[nodiscard]] static std::pair<ImVec2, ImVec2> Get_Windows_Boundaries();
        [[nodiscard]] ImVec4 Get_Person_Color(const CPerson& person);

    private:
        const CSimulation* m_simulation;
        const TConfig* m_config;
    };
}