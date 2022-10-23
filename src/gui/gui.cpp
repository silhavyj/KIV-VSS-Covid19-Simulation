#include <algorithm>
#include <atomic>
#include <thread>
#include <cmath>

#include <imgui.h>
#include <implot.h>

#include "../simulation/config.h"
#include "../simulation/person.h"
#include "../simulation/simulator.h"
#include "../simulation/singleton.h"
#include "gui.h"

namespace kiv_vss::gui
{
    static void Draw_Windows();
    static void Draw_Simulation_Window();
    static void Draw_Control_Window();
    static void Draw_Char_Window();
    static void Draw_Person(ImDrawList* draw_list, CPerson& person);
    static void Draw_Popular_Locations(ImDrawList* draw_list, CLocation& location);
    static std::pair<ImVec2, ImVec2> Get_Windows_Boundaries();
    static ImVec4 Get_Person_Color(CPerson& person);
    static void Run_Simulation();

    static ImVec4 vulnerable_color = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);
    static ImVec4 infected_color   = ImVec4(0.0f, 1.0f, 0.0f, 0.5f);
    static ImVec4 immune_color     = ImVec4(0.0f, 0.0f, 1.0f, 0.5f);
    static ImVec4 dead_color       = ImVec4(1.0f, 0.0f, 0.0f, 0.5f);

    static ImVec4 popular_location_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    static CSimulator* simulator = new CSimulator();
    static std::atomic<bool> stop_simulation{true};
    static std::mutex mtx{};
    static std::vector<double> number_of_infected_people;
    static std::vector<double> number_of_fatalities;
    static std::vector<double> number_of_immune_people;

    void Render_GUI()
    {
        static bool dockspace_open = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                ImGui::Separator();

                if (ImGui::MenuItem("Close", NULL, false))
                    dockspace_open = false;
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        Draw_Windows();
        ImGui::End();
    }

    static void Draw_Windows()
    {
        Draw_Simulation_Window();
        Draw_Control_Window();
        Draw_Char_Window();
        //ImGui::ShowDemoWindow();
    }

    static void Draw_Char_Window()
    {
        std::vector<double> data;
        std::vector<double> data2;
        std::vector<double> data3;

        mtx.lock();
        data = number_of_infected_people;
        data2 = number_of_fatalities;
        data3 = number_of_immune_people;
        mtx.unlock();

        std::vector<double> x_data(data.size());
        std::generate(x_data.begin(), x_data.end(), [n = 0]() mutable { return ++n; });

        ImGui::Begin("Statistics");
        if (ImPlot::BeginPlot("Statistics"))
        {
            ImPlot::PlotLine("Number of infected people", x_data.data(), data.data(), data.size());
            ImPlot::PlotLine("Number of fatalities", x_data.data(), data2.data(), data2.size());
            //ImPlot::PlotLine("Number of people with immunity", x_data.data(), data3.data(), data3.size());
            ImPlot::EndPlot();
        }
        ImGui::End();
    }

    static void Run_Simulation()
    {
        static size_t hours = 0;
        while (!stop_simulation)
        {
            mtx.lock();
            simulator->Update();
            if (hours % 24 == 0)
            {
                number_of_infected_people.push_back(simulator->Get_Number_Of_Infected_People());
                number_of_fatalities.push_back(simulator->Get_Number_Of_Fatalities());
                number_of_immune_people.push_back(simulator->Get_Number_Immune_People());
            }
            mtx.unlock();
            ++hours;
            std::this_thread::sleep_for(std::chrono::microseconds(200));
        }
    }

    static void Draw_Control_Window()
    {
        ImGui::Begin("Control");
        if (ImGui::Button("Start simulation"))
        {
            if (stop_simulation == true)
            {
                mtx.lock();
                delete simulator;
                simulator = new CSimulator();
                number_of_fatalities.clear();
                number_of_infected_people.clear();
                mtx.unlock();
                std::thread t(&Run_Simulation);
                t.detach();
                stop_simulation = false;
            }
        }
        if (ImGui::Button("Pause simulation"))
        {
            stop_simulation = true;
        }
        if (ImGui::Button("Resume simulation"))
        {
            std::thread t(&Run_Simulation);
            t.detach();
            stop_simulation = false;
        }
        if (ImGui::Button("Reset"))
        {
            stop_simulation = true;
            mtx.lock();
            delete simulator;
            simulator = new CSimulator();
            number_of_fatalities.clear();
            number_of_infected_people.clear();
            mtx.unlock();
        }
        ImGui::Separator();
        ImGui::Text("Number of popular places");
        ImGui::InputInt("##",reinterpret_cast<int *>(&kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Number_Of_Popular_Places));
        ImGui::Separator();
        ImGui::Text("[] of people in self isolation");
        ImGui::SliderFloat("##", &kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->Ratio_Of_People_In_Self_Isolation, 0.0, 1.0);
        ImGui::End();
    }

    static void Draw_Simulation_Window()
    {
        /*static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);*/

        ImGui::Begin("Covid19 Simulation");
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        mtx.lock();
        auto& people = simulator->Get_People();
        auto& popular_location = simulator->Get_Popular_Locations();

        std::for_each(people.begin(), people.end(), [&](auto person) { Draw_Person(draw_list, person); });
        std::for_each(popular_location.begin(), popular_location.end(), [&](auto location) { Draw_Popular_Locations(draw_list, location); });
        mtx.unlock();
        ImGui::End();
    }

    static void Draw_Person(ImDrawList* draw_list, CPerson& person)
    {
        const auto [y, x] = person.Get_Current_Location().Get_Coordinates();
        auto [v_min, v_max ] = Get_Windows_Boundaries();

        //ImGui::GetForegroundDrawList()->AddRect(v_min, v_max, IM_COL32(255, 255, 0, 255));

        v_min.y += (((v_max.y - v_min.y) * y) / kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->World_Size) - 2;
        v_min.x += (((v_max.x - v_min.x) * x) / kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->World_Size) - 2;

        draw_list->AddCircleFilled(v_min, 4, ImGui::GetColorU32(Get_Person_Color(person)));
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

    static void Draw_Popular_Locations(ImDrawList* draw_list, CLocation& location)
    {
        const auto [y, x] = location.Get_Coordinates();
        auto [v_min, v_max ] = Get_Windows_Boundaries();

        v_min.y += (((v_max.y - v_min.y) * y) / kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->World_Size) - 4;
        v_min.x += (((v_max.x - v_min.x) * x) / kiv_vss::Singleton<kiv_vss::Config>::Get_Instance()->World_Size) - 4;

        draw_list->AddCircleFilled(v_min, 8, ImGui::GetColorU32(popular_location_color));
    }

    static ImVec4 Get_Person_Color(CPerson& person)
    {
        const auto state = person.Get_Infection_State();

        switch (state)
        {
            case CPerson::NInfection_State::Immune:
                return immune_color;

            case CPerson::NInfection_State::Infected:
                return infected_color;

            case CPerson::NInfection_State::Vulnerable:
                return vulnerable_color;

            case CPerson::NInfection_State::Dead:
                return dead_color;

            default:
                return {};
        }
    }
}