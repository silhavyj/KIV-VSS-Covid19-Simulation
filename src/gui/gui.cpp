#include "imgui.h"
#include "implot.h"

#include "gui.h"
#include "../simulation/simulator.h"
#include "../simulation/singleton.h"

namespace kiv_vss::gui
{
    static void Draw_Simulation();
    static void Draw_Control_Panel();
    static void Draw_Person(CPerson& person, ImDrawList* draw_list);
    static std::pair<ImVec2, ImVec2> Get_Windows_Boundaries();

    static CSimulator* s_simulator = new CSimulator;
    static const auto s_config = Singleton<TConfig>::Get_Instance();

    void Render_GUI()
    {
        Draw_Simulation();
        Draw_Control_Panel();
    }

    static void Draw_Simulation()
    {
        ImGui::Begin("Simulation");
        const auto people = s_simulator->Get_People();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        std::for_each(people.begin(), people.end(), [&](auto person) { Draw_Person(person, draw_list); });
        ImGui::End();
    }

    static void Draw_Person(CPerson& person, ImDrawList* draw_list)
    {
        const auto [y, x] = person.Get_Current_Location().Get_Coordinates();
        auto [v_min, v_max ] = Get_Windows_Boundaries();

        v_min.y += (((v_max.y - v_min.y) * y) / s_config->World_Size) - 2;
        v_min.x += (((v_max.x - v_min.x) * x) / s_config->World_Size) - 2;

        draw_list->AddCircleFilled(v_min, 4, IM_COL32(255, 255, 0, 255));
    }

    static std::pair<ImVec2, ImVec2> Get_Windows_Boundaries()
    {
        ImVec2 v_max = ImGui::GetWindowContentRegionMax();
        ImVec2 v_min = ImGui::GetWindowContentRegionMin();

        v_min.x += ImGui::GetWindowPos().x;
        v_min.y += ImGui::GetWindowPos().y;
        v_max.x += ImGui::GetWindowPos().x;
        v_max.y += ImGui::GetWindowPos().y;

        return { v_min, v_max };
    }

    static void Draw_Control_Panel()
    {
        ImGui::Begin("Control panel");

        if (ImGui::Button("Play"))
        {
            // TODO
        }
        if (ImGui::Button("Pause/Resume"))
        {
            // TODO
        }
        if (ImGui::Button("Reset"))
        {
            delete s_simulator;
            s_simulator = new CSimulator;
        }
        ImGui::End();
    }
}