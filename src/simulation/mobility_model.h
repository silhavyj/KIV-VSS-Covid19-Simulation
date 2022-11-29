#pragma once

#include <vector>

#include "person.h"
#include "config.h"

namespace kiv_vss
{
    /// \author Jakub Silhavy
    /// This class defines the mobility model that moves
    /// the people around. It captures a simplified version
    /// of how people travel in the real word.
    class CMobility_Model
    {
    public:
        /// Creates an instance of the class.
        /// \param person Person to be moved around by the mobility model
        /// \param popular_places Popular places people tend to go
        /// \param self_isolating Flag indicating whether or not the person is self-isolating
        explicit CMobility_Model(CPerson* person, const std::vector<CLocation>* popular_places, bool self_isolating);

        /// Default destructor.
        ~CMobility_Model() = default;

        /// Updates the person's state.
        /// Each person is allowed to stay at a location for a certain amount of time.
        /// When the time is up, this method will decided the person's next location, and
        /// it moves them towards the location until they have reached the destination.
        void Update();

    private:
        /// Checks if the user is going to self-isolate due to catching the virus.
        /// Also, it checks whether the user can stop quarantining as they have recovered.
        void Check_Infection_State();

        /// Picks the next locating the person will go.
        /// \return Person's next final destination
        [[nodiscard]] CLocation Pick_Next_Location() const;

        /// Picks a random popular location.
        /// \return Arbitrary popular location.
        [[nodiscard]] CLocation Pick_Random_Popular_Place() const;

        /// Generates a random period of time for which the user
        /// will be allowed to stay at the next location.
        /// \return Random period of time to spend at a location
        [[nodiscard]] std::size_t Generate_Random_Hours_At_Location() const;

    private:
        /// Mobility state of a person
        enum class NMobility_State : std::uint8_t
        {
            Headed_To_Random_Location, ///< Person is going to an arbitrary location
            Headed_To_Popular_Place,   ///< Person is on their way to a popular location
            Headed_Home                ///< Person is going back home
        };

    private:
        CPerson* m_person;                              ///< Person to be moved around
        const std::vector<CLocation>* m_popular_places; ///< List of popular places people tend to go
        bool m_self_isolating;                          ///< Flag if the person is willingly self-isolating
        bool m_self_isolating_due_to_infection;         ///< Flag if the person is self-isolating due to infection
        const TConfig* m_config;                        ///< Configuration of the simulation
        bool m_person_is_moving;                        ///< Flag if the person si headed to a location
        CLocation m_next_location;                      ///< Location the person is going to
        std::size_t m_at_location_countdown;            ///< Countdown of how much timer the user has spent at a location
    };
}

// EOF