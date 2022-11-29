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
        // Start the window.
        ImGui::Begin("Simulation");

        // Make sure the simulation has been set.
        if (nullptr != m_simulation)
        {
            // Retrieve the people along with popular locations.
            const auto& people = m_simulation->Get_People();
            const auto& popular_locations = m_simulation->Get_Popular_Locations();

            // Create a list of drawn objects.
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            // Render the people (one by one);
            std::for_each(people.begin(), people.end(), [&](auto person) { Draw_Person(person, draw_list); });

            // Render the popular places if the user checked the checkbox.
            if (*m_display_popular_locations)
            {
                std::for_each(popular_locations.begin(), popular_locations.end(), [&](auto location) { Draw_Popular_Location(location, draw_list); });
            }

            // Check if the user clicked on the screen.
            // If they did, add a new popular location (assuming the simulation is not running).
            if (!*m_simulation_running && ImGui::IsWindowFocused() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                Add_Popular_Location();
            }
        }

        // End the window.
        ImGui::End();
    }

    void CSimulation_Window::Add_Popular_Location()
    {
        // Get the mouse positions.
        auto [mouse_x, mouse_y] = ImGui::GetMousePos();

        // Get the boundaries of the windows.
        const auto [v_min, v_max] = Get_Windows_Boundaries();

        // Calculate relative coordinates of the mouse position within the window.
        mouse_x -= v_min.x;
        mouse_y -= v_min.y;

        // Scale the coordinates down to <0; 1>.
        mouse_x /= (v_max.x - v_min.x);
        mouse_y /= (v_max.y - v_min.y);

        // Check the user clicked on the window (not outside the boundaries).
        if (mouse_x < 0.0 || mouse_x > 1.0 || mouse_y < 0.0 || mouse_y > 1.0)
        {
            return;
        }

        // Calculate the coordinates from the world's size point of view.
        mouse_x *= m_config->general.world_size;
        mouse_y *= m_config->general.world_size;

        // Add a new popular locations.
        m_simulation->Add_Popular_Location(CLocation(mouse_y, mouse_x));
    }

    void CSimulation_Window::Draw_Person(const CPerson& person, ImDrawList* draw_list)
    {
        const auto [y, x] = person.Get_Current_Location().Get_Coordinates();
        auto [v_min, v_max ] = Get_Windows_Boundaries();

        // Calculate the coordinates relatively to the window.
        v_min.y += (((v_max.y - v_min.y) * y) / m_config->general.world_size) - Person_Circle_Size / 2.0;
        v_min.x += (((v_max.x - v_min.x) * x) / m_config->general.world_size) - Person_Circle_Size / 2.0;

        // Get the corresponding color based on the infection state.
        const auto person_color = ImGui::GetColorU32(Get_Person_Color(person));

        // Draw the person.
        draw_list->AddCircleFilled(v_min, Person_Circle_Size, person_color);

        // If the person is infected, draw an outer circle around them.
        if (person.Get_Infection_State() == CPerson::NInfection_State::Infected)
        {
            draw_list->AddCircle(v_min, Person_Circle_Size * 2, person_color);
        }
    }

    void CSimulation_Window::Draw_Popular_Location(const CLocation& location, ImDrawList* draw_list)
    {
        const auto [y, x] = location.Get_Coordinates();
        auto [v_min, v_max ] = Get_Windows_Boundaries();

        // Calculate the coordinates relatively to the window.
        v_min.y += (((v_max.y - v_min.y) * y) / m_config->general.world_size);
        v_min.x += (((v_max.x - v_min.x) * x) / m_config->general.world_size);

        // Draw the popular location.
        draw_list->AddCircleFilled(v_min, Popular_Location_Circle_Size, ImGui::GetColorU32(Popular_Location_Color));
    }

    std::pair<ImVec2, ImVec2> CSimulation_Window::Get_Windows_Boundaries()
    {
        // Get the region boundaries
        ImVec2 v_max = ImGui::GetWindowContentRegionMax();
        ImVec2 v_min = ImGui::GetWindowContentRegionMin();

        // Move it relatively to the window position.
        v_min.x += ImGui::GetWindowPos().x;
        v_min.y += ImGui::GetWindowPos().y;
        v_max.x += ImGui::GetWindowPos().x;
        v_max.y += ImGui::GetWindowPos().y;

        return { v_min, v_max };
    }

    ImVec4 CSimulation_Window::Get_Person_Color(const CPerson& person)
    {
        // Get the infection state of the person.
        const auto state = person.Get_Infection_State();

        switch (state)
        {
            // Immunity color
            case CPerson::NInfection_State::Immune:
                return {0.0f, 0.8f, 1.0f, 0.5f};

            // Infected color
            case CPerson::NInfection_State::Infected:
                return {0.0f, 1.0f, 0.0f, 0.5f};

            // Vulnerable color
            case CPerson::NInfection_State::Susceptible:
                return {0.5f, 0.5f, 0.5f, 0.5f};

            // Dead color
            case CPerson::NInfection_State::Dead:
                return {1.0f, 0.0f, 0.0f, 0.5f};

            // Should not happen
            default:
                spdlog::error("Person's color could not be determined. Returning {} as the default color.");
                return {};
        }
    }
}

// EOF