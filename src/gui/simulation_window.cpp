#include "spdlog/spdlog.h"

#include "simulation_window.h"

namespace kiv_vss::gui
{
    CSimulation_Window::CSimulation_Window(CSimulation* simulation, const bool* display_popular_locations, const bool* simulation_running)
        : GUI_Window(simulation),
          m_display_popular_locations{display_popular_locations},
          m_simulation_running{simulation_running}
    {

    }

    void CSimulation_Window::Render()
    {
        ImGui::Begin("Simulation");

        if (nullptr != m_simulation)
        {
            const auto& people = m_simulation->Get_People();
            const auto& popular_locations = m_simulation->Get_Popular_Locations();

            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            std::for_each(people.begin(), people.end(), [&](auto person) { Draw_Person(person, draw_list); });

            if (*m_display_popular_locations)
            {
                std::for_each(popular_locations.begin(), popular_locations.end(), [&](auto location) { Draw_Popular_Location(location, draw_list); });
            }

            if (!*m_simulation_running && ImGui::IsWindowFocused() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                Add_Popular_Location();
            }
        }

        ImGui::End();
    }

    void CSimulation_Window::Add_Popular_Location()
    {
        auto [mouse_x, mouse_y] = ImGui::GetMousePos();
        const auto [v_min, v_max] = Get_Windows_Boundaries();

        mouse_x -= v_min.x;
        mouse_y -= v_min.y;

        mouse_x /= (v_max.x - v_min.x);
        mouse_y /= (v_max.y - v_min.y);

        if (mouse_x < 0.0 || mouse_x > 1.0 || mouse_y < 0.0 || mouse_y > 1.0)
        {
            return;
        }

        mouse_x = std::max(0.0f, std::min(1.0f, mouse_x));
        mouse_y = std::max(0.0f, std::min(1.0f, mouse_y));

        mouse_x *= m_config->general.world_size;
        mouse_y *= m_config->general.world_size;

        m_simulation->Add_Popular_Location(CLocation(mouse_y, mouse_x));
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

    void CSimulation_Window::Draw_Popular_Location(const CLocation& location, ImDrawList* draw_list)
    {
        const auto [y, x] = location.Get_Coordinates();
        auto [v_min, v_max ] = Get_Windows_Boundaries();

        v_min.y += (((v_max.y - v_min.y) * y) / m_config->general.world_size);
        v_min.x += (((v_max.x - v_min.x) * x) / m_config->general.world_size);

        draw_list->AddCircleFilled(v_min, Popular_Location_Circle_Size, ImGui::GetColorU32(Popular_Location_Color));
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
                return {0.0f, 0.8f, 1.0f, 0.5f};

            case CPerson::NInfection_State::Infected:
                return {0.0f, 1.0f, 0.0f, 0.5f};

            case CPerson::NInfection_State::Susceptible:
                return {0.5f, 0.5f, 0.5f, 0.5f};

            case CPerson::NInfection_State::Dead:
                return {1.0f, 0.0f, 0.0f, 0.5f};

            default:
                spdlog::error("Person's color could not be determined. Returning {} as the default color.");
                return {};
        }
    }
}