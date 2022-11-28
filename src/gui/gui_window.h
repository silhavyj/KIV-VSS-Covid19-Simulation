#pragma once

#include "imgui.h"

#include "../simulation/config.h"
#include "../simulation/simulation.h"

namespace kiv_vss::gui
{
    /// \author Jakub Silhavy
    /// This class represents a parent class for all GUI windows.
    /// It defines method that each inherited class needs to implement.
    /// It also encapsulates parameters used by all windows.
    class GUI_Window
    {
    public:
        /// Creates an instance of the class
        /// \param simulation Backend of the simulation
        explicit GUI_Window(CSimulation* simulation);

        /// Default virtual destructor.
        virtual ~GUI_Window() = default;

        /// Sets the simulation.
        /// This method is called whenever a simulation gets resets
        /// (a new instance of CSimulation is created).
        /// \param simulation Backend of the simulation
        void Set_Simulation(CSimulation* simulation);

        /// Renders the window.
        virtual void Render() = 0;

    protected:
        CSimulation* m_simulation; ///< Simulation (backend logic)
        TConfig* m_config;         ///< Configuration of the simulation
    };
}

// EOF