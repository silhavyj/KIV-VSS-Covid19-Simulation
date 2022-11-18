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

        ImGui::SliderInt("Number of initially infected people", reinterpret_cast<int *>(&m_config->general.number_of_initially_infected_people), 1, 20);
        ImGui::SliderFloat("% of self isolating people", reinterpret_cast<float *>(&m_config->general.ratio_of_people_in_self_isolation), 0.0f, 1.0f);

        ImGui::End();
    }
}