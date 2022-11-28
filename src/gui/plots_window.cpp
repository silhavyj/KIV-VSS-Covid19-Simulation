#include "implot.h"

#include "plots_window.h"

namespace kiv_vss::gui
{
    CPlots_Window::CPlots_Window(CSimulation* simulation)
        : GUI_Window(simulation)
    {

    }

    void CPlots_Window::Render()
    {
        // Start the window.
        ImGui::Begin("Disease progression over time");

        // Retrieve statistics (data to be plotted out).
        const auto& statistics = m_simulation->Get_Statistics();

        // Boolean values indicating whether or not individual charts should be displayed.
        static bool s_display_number_of_infected_people{true};
        static bool s_display_number_of_fatalities{false};
        static bool s_display_number_of_immune_people{false};
        static bool s_display_number_of_susceptible_people{false};

        // Render control checkboxes.
        ImGui::Checkbox("Number of infected people", &s_display_number_of_infected_people);
        ImGui::SameLine();
        ImGui::Checkbox("Number of fatalities", &s_display_number_of_fatalities);
        ImGui::SameLine();
        ImGui::Checkbox("Number of immune people", &s_display_number_of_immune_people);
        ImGui::SameLine();
        ImGui::Checkbox("Number of susceptible people", &s_display_number_of_susceptible_people);

        if (ImPlot::BeginPlot("Plots"))
        {
            // Render chart "Number of infected people"
            if (s_display_number_of_infected_people)
            {
                ImPlot::PlotLine(
                    "Number of infected people",
                    statistics.time.data(),
                    statistics.number_of_infected_people.data(),
                    statistics.time.size()
                );
            }

            // Render chart "Number of fatalities"
            if (s_display_number_of_fatalities)
            {
                ImPlot::PlotLine(
                    "Number of fatalities",
                    statistics.time.data(),
                    statistics.number_of_fatalities.data(),
                    statistics.time.size()
                );
            }

            // Render chart "Number of immune people"
            if (s_display_number_of_immune_people)
            {
                ImPlot::PlotLine(
                    "Number of immune people",
                    statistics.time.data(),
                    statistics.number_of_immune_people.data(),
                    statistics.time.size()
                );
            }

            // Render chart "Number of susceptible people"
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

        // End the window.
        ImGui::End();
    }
}

// EOF