#pragma once

#include "gui_window.h"

namespace kiv_vss::gui
{
    /// \author Jakub Silhavy
    /// This class renders the Settings window.
    /// The user uses this window to change the parameters of the simulation.
    class CSettings_Window : public GUI_Window
    {
    public:
        /// Creates an instance of the class. 
        /// \param simulation Backend logic of the simulation
        /// \param simulating_running Flag indicating whether the simulation is running (no real-time change of settings)
        explicit CSettings_Window(CSimulation* simulation, const bool* simulating_running);

        /// Default destructor.
        ~CSettings_Window() override = default;

        /// Renders the window.
        void Render() override;

    private:
        /// Renders the settings.
        void Render_Settings() const;

        /// Renders a message saying that the user cannot change the settings while the simulation is running.
        void Render_Simulation_Running_Message() const;

        /// Renders a general setting tab.
        void Render_General_Settings() const;

        /// Renders a disease settings tab. 
        void Render_Disease_Settings() const;

        /// Renders a mobility settings tab.
        void Render_Mobility_Settings() const;

    private:
        const bool* m_simulating_running; ///< Flag indicating if the simulation is running or not
    };
}

// EOF