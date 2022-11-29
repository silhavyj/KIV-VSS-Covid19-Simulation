#pragma once

#include "gui_window.h"

namespace kiv_vss::gui
{
    /// \author Jakub Silhavy
    /// This class represents the windows that displays
    /// statistics calculated throughout the simulation.
    class CStatistics_Window : public GUI_Window
    {
    public:
        /// Creates an instance of the class.
        /// \param simulation Backend logic of the simulation
        explicit CStatistics_Window(CSimulation* simulation);

        /// Default destructor.
        ~CStatistics_Window() override = default;

        /// Renders the window.
        void Render() override;

    private:
        /// Calculates statistical value (number of infected people, number of immune people, ...)
        /// into a percentage value (0 - 100%), where 100% is the size of the entire population.
        /// \param data Last value in this vector is used to calculate the value (latest measurement - data.back())
        /// \return Corresponding percentage value
        [[nodiscard]] double Get_Number_Of_People_Percentage(const std::vector<float>& data) const noexcept;
        
        /// Calculates percentage with regards to the total number of people.
        /// \param value Value to be converted into percentage (0 - 100%)
        /// \return Corresponding percentage value
        [[nodiscard]] double Get_Percentage_People(double value) const noexcept;
    };
}

// EOF