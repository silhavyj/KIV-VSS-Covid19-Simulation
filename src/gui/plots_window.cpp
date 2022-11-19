#include "implot.h"

#include "plots_window.h"

namespace kiv_vss::gui
{
    CPlots_Window::CPlots_Window(const CSimulation* simulation)
        : GUI_Window(simulation)
    {

    }

    void CPlots_Window::Render()
    {
        ImGui::Begin("Disease progression over time");

        const auto& statistics = m_simulation->Get_Statistics();

        static bool s_display_number_of_infected_people{true};
        static bool s_display_number_of_fatalities{false};
        static bool s_display_number_of_immune_people{false};
        static bool s_display_number_of_susceptible_people{false};

        ImGui::Checkbox("Number of infected people", &s_display_number_of_infected_people);
        ImGui::SameLine();
        ImGui::Checkbox("Number of fatalities", &s_display_number_of_fatalities);
        ImGui::SameLine();
        ImGui::Checkbox("Number of immune people", &s_display_number_of_immune_people);
        ImGui::SameLine();
        ImGui::Checkbox("Number of susceptible people", &s_display_number_of_susceptible_people);

        if (ImPlot::BeginPlot("Plots"))
        {
            if (s_display_number_of_infected_people)
            {
                ImPlot::PlotLine(
                    "Number of infected people",
                    statistics.time.data(),
                    statistics.number_of_infected_people.data(),
                    statistics.time.size()
                );
            }

            if (s_display_number_of_fatalities)
            {
                ImPlot::PlotLine(
                    "Number of fatalities",
                    statistics.time.data(),
                    statistics.number_of_fatalities.data(),
                    statistics.time.size()
                );
            }

            if (s_display_number_of_immune_people)
            {
                ImPlot::PlotLine(
                    "Number of immune people",
                    statistics.time.data(),
                    statistics.number_of_immune_people.data(),
                    statistics.time.size()
                );
            }

            if (s_display_number_of_susceptible_people)
            {
                ImPlot::PlotLine(
                    "Number of susceptible people",
                    statistics.time.data(),
                    statistics.number_of_susceptible_people.data(),
                    statistics.time.size()
                );
            }

            ImPlot::EndPlot();
        }

        ImGui::End();
    }
}