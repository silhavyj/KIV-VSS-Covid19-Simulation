#pragma once

#include "gui_window.h"

namespace kiv_vss::gui
{
    /// \author Jakub Silhavy
    /// This class renders the window that displays plots.
    class CPlots_Window : public GUI_Window
    {
    public:
        /// Creates an instance of the class.
        /// \param simulation Backend logic of the simulation
        explicit CPlots_Window(CSimulation* simulation);

        /// Default destructor.
        ~CPlots_Window() override = default;

        /// Renders the window.
        void Render() override;
    };
}

// EOF