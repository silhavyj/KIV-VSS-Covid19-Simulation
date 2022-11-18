#include "spdlog/spdlog.h"

#include "simulation_window.h"

namespace kiv_vss::gui
{
    CSimulation_Window::CSimulation_Window(const CSimulation* simulation)
        : GUI_Window(simulation)
    {

    }

    void CSimulation_Window::Render()
    {
        ImGui::Begin("Simulation");

        if (nullptr != m_simulation)
        {
            const auto& people = m_simulation->Get_People();
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            std::for_each(people.begin(), people.end(), [&](auto person) { Draw_Person(person, draw_list); });

            // TODO add a popular location when the mouse is clicked
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                const auto [x, y] = ImGui::GetCursorScreenPos();
                //spdlog::info("[{}, {}]", y, x);
            }
        }

        ImGui::End();
    }

    void CSimulation_Window::Draw_Person(const CPerson& person, ImDrawList* draw_list)
    {
        const auto [y, x] = person.Get_Current_Location().Get_Coordinates();
        auto [v_min, v_max ] = Get_Windows_Boundaries();

        v_min.y += (((v_max.y - v_min.y) * y) / m_config->general.world_size) - Person_Circle_Size / 2.0;
        v_min.x += (((v_max.x - v_min.x) * x) / m_config->general.world_size) - Person_Circle_Size / 2.0;

        const auto person_color = ImGui::GetColorU32(Get_Person_Color(person));
        draw_list->AddCircleFilled(v_min, Person_Circle_Size, person_color);
        if (person.Get_Infection_State() == CPerson::NInfection_State::Infected)
        {
            draw_list->AddCircle(v_min, Person_Circle_Size * 2, person_color);
        }
    }

    std::pair<ImVec2, ImVec2> CSimulation_Window::Get_Windows_Boundaries()
    {
        ImVec2 v_max = ImGui::GetWindowContentRegionMax();
        ImVec2 v_min = ImGui::GetWindowContentRegionMin();

        v_min.x += ImGui::GetWindowPos().x;
        v_min.y += ImGui::GetWindowPos().y;
        v_max.x += ImGui::GetWindowPos().x;
        v_max.y += ImGui::GetWindowPos().y;

        return { v_min, v_max };
    }

    ImVec4 CSimulation_Window::Get_Person_Color(const CPerson& person)
    {
        const auto state = person.Get_Infection_State();

        switch (state)
        {
            case CPerson::NInfection_State::Immune:
                return ImVec4(0.0f, 0.8f, 1.0f, 0.5f);

            case CPerson::NInfection_State::Infected:
                return ImVec4(0.0f, 1.0f, 0.0f, 0.5f);

            case CPerson::NInfection_State::Susceptible:
                return ImVec4(0.5f, 0.5f, 0.5f, 0.5f);

            case CPerson::NInfection_State::Dead:
                return ImVec4(1.0f, 0.0f, 0.0f, 0.5f);

            default:
                spdlog::error("Person's color could not be determined. Returning {} as the default color.");
                return {};
        }
    }
}