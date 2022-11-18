#include "statistics_window.h"

namespace kiv_vss::gui
{
    CStatistics_Window::CStatistics_Window(const CSimulation* simulation)
        : GUI_Window(simulation)
    {

    }

    void CStatistics_Window::Render()
    {
        ImGui::Begin("Statistics");

        const auto& statistics = m_simulation->Get_Statistics();

        ImGui::Text("Currently infected people = %.2f", Get_Number_Of_People_Percentage(statistics.number_of_infected_people));
        ImGui::Text("Currently immune people = %.2f", Get_Number_Of_People_Percentage(statistics.number_of_immune_people));
        ImGui::Text("Total fatality rate = %.2f", Get_Number_Of_People_Percentage(statistics.number_of_fatalities));
        ImGui::Text("Currently susceptible people = %.2f", Get_Number_Of_People_Percentage(statistics.number_of_susceptible_people));
        ImGui::Text("System saturated = %s", m_simulation->Is_System_Saturated() ? "YES\0" : "NO\0");

        ImGui::Separator();

        // TODO print out peaks

        ImGui::End();
    }

    inline double CStatistics_Window::Get_Number_Of_People_Percentage(const std::vector<float>& data) const
    {
        if (data.empty())
        {
            return 0.0;
        }
        return 100.0 * data.back() / m_config->general.number_of_people;
    }
}