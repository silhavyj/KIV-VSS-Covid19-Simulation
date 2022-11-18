#include "imgui.h"

#include "settings.h"
#include "../simulation/singleton.h"

namespace kiv_vss::gui
{
    CSettings_Window::CSettings_Window()
        : m_config{Singleton<TConfig>::Get_Instance()}
    {

    }

    void CSettings_Window::Render()
    {
        ImGui::Begin("Settings");

        ImGui::SliderInt("Number of initially infected people", reinterpret_cast<int *>(&m_config->layout.number_of_initially_infected_people), 1, 20);
        ImGui::SliderFloat("% of self isolating people", reinterpret_cast<float *>(&m_config->layout.ratio_of_people_in_self_isolation), 0.0f, 1.0f);

        ImGui::End();
    }
}