#pragma once

#include "gui_window.h"

namespace kiv_vss::gui
{
    /// \author Jakub Silhavy
    /// This class represents the simulation window.
    class CSimulation_Window : public GUI_Window
    {
    public:
        /// Creates an instance of the window 
        /// \param simulation Backend logic of the simulation
        /// \param display_popular_locations Flag indicating if the popular locations should be displayed
        /// \param simulation_running Flag indicating the simulating is running
        explicit CSimulation_Window(CSimulation* simulation, const bool* display_popular_locations, const bool* simulation_running);

        /// Default destructor.
        ~CSimulation_Window() override = default;

        /// Renders the window.
        void Render() override;

    private:
        static constexpr float Person_Circle_Size = 4.0f;                               ///< Circle size representing a person
        static constexpr float Popular_Location_Circle_Size = 2 * Person_Circle_Size;   ///< Circle size representing a popular location
        static constexpr auto Popular_Location_Color = ImVec4(1.0f, 1.0f, 0.0f, 0.75f); ///< Circle color representing a popular location

    private:
        /// Renders a single person.
        /// \param person Person to be rendered out
        /// \param draw_list List of drawn objects
        void Draw_Person(const CPerson& person, ImDrawList* draw_list);

        /// Renders a popular location.
        /// \param location Location to be rendered out
        /// \param draw_list List of drawn objects
        void Draw_Popular_Location(const CLocation& location, ImDrawList* draw_list);

        /// Returns window boundaries.
        /// This method is used when calculating coordinates of
        /// people of popular locations.
        /// \return Window boundaries [min; max]
        [[nodiscard]] static std::pair<ImVec2, ImVec2> Get_Windows_Boundaries();

        /// Returns person's color.
        /// The color depends on the infection state of the person.
        /// \param person Person whose color is to be determined
        /// \return Corresponding color (based on the infection state)
        [[nodiscard]] static ImVec4 Get_Person_Color(const CPerson& person);

        /// Adds a popular location to the simulation.
        /// This method is triggered when the user clicks on the screen.
        void Add_Popular_Location();

    private:
        const bool* m_display_popular_locations; ///< Flag indicating if the popular locations should be displayed
        const bool* m_simulation_running;        ///< Flag indicating the simulating is running
    };
}

// EOF