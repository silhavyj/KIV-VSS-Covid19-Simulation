#pragma once

#include <vector>

#include "config.h"
#include "person.h"
#include "mobility_model.h"
#include "infection_model.h"

namespace kiv_vss
{
    /// \author Jakub Silhavy
    /// This class represents a Coronavirus outbreak simulation.
    /// It provides method to control the simulation as well as
    /// methods to retrieve statistical information about the simulation.
    class CSimulation
    {
    public:
        /// A single statistical record (what is being observed).
        struct TStatistics_Record
        {
            float number_of_fatalities{0};               ///< Number who have died from the disease
            float number_of_infected_people{0};          ///< Number of infected people
            float number_of_immune_people{0};            ///< Number of immune people
            float number_of_susceptible_people{0};       ///< Number of susceptible people
            double number_of_infections_per_person{0.0}; ///< How many times each person has been infected (average)
        };

        /// Statistical values captured overtime
        struct TStatistics
        {
            std::vector<float> number_of_fatalities;              ///< Sampled number of dead people overtime
            std::vector<float> number_of_infected_people;         ///< Sampled number of infected people overtime
            std::vector<float> number_of_immune_people;           ///< Sampled number of immune people overtime
            std::vector<float> number_of_susceptible_people;      ///< Sampled number of susceptible people overtime
            std::vector<float> number_of_contractions_per_person; ///< How many times each person was infected overtime (average)
            std::vector<float> time;                              ///< Time - number of updates (x axes)
        };

    public:
        /// Creates an instance of the class.
        CSimulation();

        /// Default destructor.
        ~CSimulation() = default;

        /// Updates the simulation.
        /// Moves the simulation forward in time by one hour.
        void Update();

        /// Adds a popular location to the list of popular places.
        /// This method is called from the GUI when the user picks a location of a popular place (e.g. a restaurant)
        /// \param Popular location to be added to the list.
        void Add_Popular_Location(const CLocation& location);

        /// Returns measured statistical values since the beginning of the simulation.
        /// \return Statistical values.
        [[nodiscard]] const TStatistics& Get_Statistics() const noexcept;

        /// Returns all people in the population.
        /// This method is called from the GUI for visualization purposes.
        /// \return People (population)
        [[nodiscard]] const std::vector<CPerson>& Get_People() const noexcept;

        /// Returns the list of popular locations.
        /// This method is called from the GUI for visualization purposes.
        /// \return List of popular places
        [[nodiscard]] const std::vector<CLocation>& Get_Popular_Locations() const noexcept;

        /// Returns the list of popular locations.
        /// This method is called from the GUI for visualization purposes.
        /// \return List of popular places
        [[nodiscard]] std::vector<CLocation>& Get_Popular_Locations() noexcept;

        /// Returns whether the health case system is saturated or not.
        /// The system is saturated when at least 20% of people are sick at once.
        /// However, this parameter is adjustable through the GUI.
        /// \return True, if the system is saturated, false otherwise.
        [[nodiscard]] bool Is_System_Saturated() const noexcept;

        /// Returns whether the simulation is over or not.
        /// The simulation is over when there are not sick people in the population.
        /// \return True, if the simulation is over, false otherwise.
        [[nodiscard]] bool Is_Simulation_Over() const noexcept;
        
        /// Returns the maximum (peek) of the infected people throughout the simulation.
        /// \return Maximum people that have been infected at once.
        [[nodiscard]] std::size_t Get_Maximum_Number_Of_Infected_People() const noexcept;

    private:
        /// Adds new values (a single record) to the overall statistics
        /// \param record Current state of the simulation (the number of infected people, ...)
        void Update_Statistics(const TStatistics_Record& record);

        /// Updates a statistical record (values measured within a single step of the simulation).
        /// \param record Record to be updated
        /// \param person Person the is going to be added to the statistical values (their infection state)
        /// \param infected_count How many time the person caught the virus so far
        void Update_Statistics_Record(TStatistics_Record& record, const CPerson& person, std::size_t infected_count);
        
        /// Generates a population (people and their homes).
        void Generate_Population();

        /// Moves people around when the simulation is updated.
        void Move_People_Around();

        /// Spreads the virus among people when the simulation is updated.
        void Spread_Virus();

        /// Tests whether an infected person infects a healthy person.
        /// \param infected_person_manager Infection model managing the infected person
        /// \param vulnerable_person_manager Infection model managing the vulnerable person
        /// \return True, if the vulnerable person gets infected, false otherwise.
        [[nodiscard]] bool Try_To_Infect(const CInfection_Model& infected_person_manager, const CInfection_Model& vulnerable_person_manager) const;

    private:
        std::vector<CPerson> m_people;                      ///< Population 
        std::vector<CLocation> m_popular_locations;         ///< List of popular places people tend to go to
        const TConfig* m_config;                            ///< Configuration of the simulation
        std::vector<CMobility_Model> m_mobility_managers;   ///< Mobility models (managers) moving people around
        std::vector<CInfection_Model> m_infection_managers; ///< Infection models (managers) 
        TStatistics m_statistics;                           ///< Statistical values measured throughout the simulation
        std::size_t m_time;                                 ///< Time (it increases with each update of the simulation)
        bool m_system_saturated;                            ///< Flag indicating whether the system is saturated or not
        std::size_t m_max_number_of_infected_people;        ///< Peek in the number of infected people
    };
}

// EOF