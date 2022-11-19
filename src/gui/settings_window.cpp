#include "settings_window.h"

namespace kiv_vss::gui
{
    CSettings_Window::CSettings_Window(const CSimulation* simulation)
        : GUI_Window(simulation)
    {

    }

    void CSettings_Window::Render()
    {
        ImGui::Begin("Settings");
        if (ImGui::BeginTabBar("tabs"))
        {
            if (ImGui::BeginTabItem("General"))
            {
                Render_General_Settings();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Disease"))
            {
                Render_Disease_Settings();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Mobility"))
            {
                Render_Mobility_Settings();
                ImGui::EndTabItem();
            }
        }
        ImGui::End();
    }

    void CSettings_Window::Render_General_Settings() const
    {
        ImGui::SliderInt("Number of people", reinterpret_cast<int *>(&m_config->general.number_of_people), 10, 1500);
        ImGui::SliderInt("Number of initially infected people", reinterpret_cast<int *>(&m_config->general.number_of_initially_infected_people), 1, 20);
        ImGui::SliderFloat("% of self isolating people", &m_config->general.ratio_of_people_in_self_isolation, 0.0f, 1.0f);
        ImGui::SliderFloat("Saturation level", &m_config->general.saturation_level, 0.0f, 1.0f);
    }

    void CSettings_Window::Render_Disease_Settings() const
    {
        ImGui::SliderFloat("Transmission distance", &m_config->disease.transmission_distance, 0.0f, 10.0f);
        ImGui::SliderFloat("Transmission probability on move", &m_config->disease.transmission_prob_on_move, 0.0f, 1.0f);
        ImGui::SliderFloat("Transmission probability at home", &m_config->disease.transmission_prob_at_home, 0.0f, 1.0f);

        ImGui::Separator();

        ImGui::SliderFloat("Death probability", &m_config->disease.death_prob, 0.0f, 1.0f);
        ImGui::SliderFloat("Death probability when saturated", &m_config->disease.death_saturated_prob, 0.0f, 1.0f);

        ImGui::Separator();

        ImGui::SliderFloat("Self-isolating when infected", &m_config->disease.self_isolating_when_infected, 0.0f, 1.0f);

        ImGui::Separator();

        ImGui::SliderInt("Average infection period [days]", reinterpret_cast<int *>(&m_config->disease.average_infection_period), 1, 30);
        ImGui::SliderInt("Average immunity period [days]", reinterpret_cast<int *>(&m_config->disease.average_immunity_period), 1, 90);
    }

    void CSettings_Window::Render_Mobility_Settings() const
    {
        ImGui::SliderFloat("Average person's speed", &m_config->mobility.average_person_speed, 1.0f, 20.0f);
        ImGui::SliderFloat("Variance of person's speed", &m_config->mobility.variance_person_speed, 1.0f, 5.0f);

        ImGui::Separator();

        ImGui::SliderInt("Maximum hours spent at a location", reinterpret_cast<int *>(&m_config->mobility.max_hours_spent_at_location), 1, 72);

        ImGui::Separator();

        ImGui::Text("Self-isolation");
        ImGui::InputFloat("Go to a popular place (prob) [1]", &m_config->mobility.isolation.go_to_popular_location_prob, 0.1);
        ImGui::InputFloat("Go to a random location (prob) [1]", &m_config->mobility.isolation.go_to_random_location_prob, 0.1);
        ImGui::InputFloat("Go home (prob) [1]", &m_config->mobility.isolation.go_home_prob, 0.1);

        ImGui::Separator();

        ImGui::Text("\"Free\"");
        ImGui::InputFloat("Go to a popular place (prob) [2]", &m_config->mobility.non_isolation.go_to_popular_location_prob, 0.1);
        ImGui::InputFloat("Go to a random location (prob) [2]", &m_config->mobility.non_isolation.go_to_random_location_prob, 0.1);
        ImGui::InputFloat("Go home (prob) [2]", &m_config->mobility.non_isolation.go_home_prob, 0.1);
    }
}