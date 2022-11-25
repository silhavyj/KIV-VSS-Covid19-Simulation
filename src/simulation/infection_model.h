#pragma once

#include "person.h"
#include "config.h"

namespace kiv_vss
{
    /// \author Jakub Silhavy
    /// This class defines an infection model used to update
    /// the health state of each person. It does not do anything
    /// unless the person is either infected or has developed immunity.
    class CInfection_Model
    {
    public:
        /// Creates an instance of the class.
        /// \param person Person managed by the infection model class
        /// \param system_saturated Indication whether or not the health care system is saturated
        /// \param initially_infected Flag if the person is one of the "lucky" people who are initially infected
        explicit CInfection_Model(CPerson* person, bool* system_saturated, bool initially_infected = false);

        /// Default destructor.
        ~CInfection_Model() = default;

        /// Infects the person.
        /// This method is called from the simulated when they happen to be
        /// within a close distance of an infected person.
        void Infect();

        /// Updates the state of the person.
        /// It decrements the counter. If the person is immune and the counter hits 0, the person
        /// becomes vulnerable to the virus again. If they are infected and the counter hits, they
        /// either develop immunity or dies.
        void Update();

        /// Returns the person managed by the infection model class.
        /// \return Person managed by the instance of this class.
        [[nodiscard]] const CPerson* Get_Person() const noexcept;

        /// Returns how many times the person has been infected so far.
        /// \return Number of times the person has caught the virus.
        [[nodiscard]] std::size_t Get_Infection_Count() const noexcept;

    private:
        /// Generates a random infection period as defined by the configuration.
        /// \return A random period of time for which the user will be contagious [hours].
        [[nodiscard]] std::size_t Generate_Random_Infection_Period() const;

        /// Generates a random immunity period as defined by the configuration.
        /// \return A random period of time for which the user will have immunity [hours].
        [[nodiscard]] std::size_t Generate_Random_Immunity_Period() const;

    private:
        CPerson* m_person;               ///< Person managed by the instance of the class
        const TConfig* m_config;         ///< Configuration of the simulation
        std::size_t m_infection_counter; ///< Number of time the person has been infected with the virus
        std::size_t m_counter;           ///< Infection/Immunity counter
        const bool* m_system_saturated;  ///< Flag if the health care system is saturated (affect death probability)
    };
}

// EOF