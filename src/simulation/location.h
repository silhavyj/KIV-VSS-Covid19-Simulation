#pragma once

#include <utility>
#include <iostream>

namespace kiv_vss
{
    /// \author Jakub Silhavy
    /// This class represents a location on a 2D grid.
    /// It defines helper function used when working with a location
    /// such as comparing, moving the location toward another location, etc.
    class CLocation
    {
    public:
        /// Simplification of a point.
        using Point = std::pair<double, double>;

    public:
        /// Creates an instance of the class.
        /// \param y Y position
        /// \param x X position
        explicit CLocation(double y, double x) noexcept;

        /// Default copy constructor.  
        /// \param location Location a new location will be instantiated off of
        CLocation(const CLocation& location) = default;

        /// Default destructor.
        ~CLocation() = default;

        /// Default assign operator.
        /// \param location Location a new location will be instantiated off of
        /// \return New instance of the Location class with the same values as
        ///         the location passed in as a parameter.
        [[nodiscard]] CLocation& operator=(const CLocation& location) = default;

        /// Overloaded subtract operator.
        /// It return the distance between this location and a location passed in as a parameter.
        /// \param location Location to which the distance will be calculated
        /// \return Distance between the two locations
        [[nodiscard]] double operator-(const CLocation& location) const;

        /// Compares two locations.
        /// It first calculates the distance between the two locations. 
        /// If the distance is less than EPSION, they will be considered to be the same.
        /// \param location Other location this location will be compared to
        /// \return True, if the two locations are the same. False, otherwise.
        [[nodiscard]] bool operator==(const CLocation& location) const;

        /// Checks if two locations are not the same.
        /// It uses operator== to find out if the two locations are the same or not.
        /// \param location Other location this location will be compared to
        /// \return True, if the two locations are NOT the same. False, otherwise.
        [[nodiscard]] bool operator!=(const CLocation& location) const;

        /// Return the Y coordinate of the location.
        /// \return Y coordinate
        [[nodiscard]] double Get_Y() const noexcept;

        /// Return the X coordinate of the location.
        /// \return X coordinate
        [[nodiscard]] double Get_X() const noexcept;

        /// Returns both coordinates ([Y; X]).
        /// @return Both coordinates of the location
        [[nodiscard]] Point Get_Coordinates() const noexcept;

        /// Moves the location toward another location by a certain distance.
        /// \param location Location to which this location will be moved.
        /// \param distance Distance by which the location will be moved.
        void Move_Toward(const CLocation& location, double distance);

        /// Generates a random location on a 2D square grid.
        /// \param min Min coordinate (in both axes)
        /// \param max Max coordinate (in both axes)
        /// \return Random location
        static CLocation Generate_Random_In_Square_Location(double min, double max);

        /// Prints out a text representation into an output stream given as a parameter.
        /// \param out Output stream the text representation of the location will be printed into.
        /// \param location Location to tbe printed out
        /// \return Output stream.
        friend std::ostream& operator<<(std::ostream& out, const CLocation& location);

        /// Returns a string representation of a Location.
        /// \param location Location a text representation will be created out of.
        /// \return Text representation of the location.
        friend std::string to_string(const CLocation& location);

    private:
        /// Subtracts coordinates of this location and the location given as a parameter.
        /// @param location Location used in the calculation
        /// @return Difference between the coordinates of the two locations
        [[nodiscard]] Point Subtract_Coordinates(const CLocation& location) const noexcept;

    private:
        double m_y; ///< Y coordinate of the location
        double m_x; ///< X coordinate of the location
    };
}

// EOF