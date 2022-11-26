#pragma once

#include "config.h"
#include "location.h"

namespace kiv_vss
{
    /// \author Jakub Silhavy
    /// This class represents a person (a single member of the population).
    /// It holds information such as the current location, home location, or
    /// the infection state.
    class CPerson
    {
    public:
        /// Infection state of a person.
        enum class NInfection_State : std::uint8_t
        {
            Susceptible, ///< Susceptible to the virus
            Infected,    ///< Person has contracted the virus
            Immune,      ///< Person has developed immunity
            Dead         ///< Person has died from disease 
        };

    public:
        /// Creates an instance of the class. 
        /// \param home Location of the person's home
        explicit CPerson(const CLocation& home);

        /// Default destructor.
        ~CPerson() = default;

        /// Returns the current person's location.
        /// \return Current location
        [[nodiscard]] const CLocation& Get_Current_Location() const noexcept;

        /// Returns the location of person's home
        /// \return Home location
        [[nodiscard]] const CLocation& Get_Home_Location() const noexcept;

        /// Returns whether the person is at home or not.
        /// \return True, if the person is at home, false otherwise.
        [[nodiscard]] bool Is_Home() const;

        /// Sets person's infection state.
        /// \param infection_state New infection state of the person
        void Set_Infection_State(NInfection_State infection_state) noexcept;

        /// Returns person's infection state. 
        /// \return Infection state of the person.
        [[nodiscard]] NInfection_State Get_Infection_State() const noexcept;

        /// Moves the person toward a location given as a parameter.
        /// How far the person will move is determined by their speed.
        /// \param location Location the person will be moved toward.
        void Move_Toward(const CLocation& location);

    private:
        /// Generates a random speed based on the configuration of the simulation.
        /// \return Random person's speed
        [[nodiscard]] double Generate_Random_Speed() const;

    private:
        CLocation m_home_location;          ///< Home location
        CLocation m_current_location;       ///< Current location
        NInfection_State m_infection_state; ///< Infection state of the person
        const TConfig* m_config;            ///< Configuration of the simulation
        double m_speed;                     ///< Person's speed (how far they can move within a single update of the simulation)
    };
}

// EOF