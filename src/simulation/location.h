#pragma once

#include <utility>
#include <iostream>

namespace kiv_vss
{
    class CLocation
    {
    public:
        using Point = std::pair<double, double>;

    public:
        explicit CLocation(double y, double  x);
        CLocation(const CLocation& location) = default;
        ~CLocation() = default;

        [[nodiscard]] CLocation& operator=(const CLocation& location) = default;
        [[nodiscard]] double operator-(const CLocation& location) const;
        [[nodiscard]] bool operator==(const CLocation& location) const;
        [[nodiscard]] bool operator!=(const CLocation& location) const;

        [[nodiscard]] double Get_Y() const;
        [[nodiscard]] double Get_X() const;
        [[nodiscard]] Point Get_Coordinates() const;

        void Move_Toward(const CLocation& location, double distance);
        static CLocation Generate_Random_In_Square_Location(double min, double max);

        friend std::ostream& operator<<(std::ostream& out, const CLocation& location);
        friend std::string to_string(const CLocation& location);

    private:
        [[nodiscard]] Point Subtract_Coordinates(const CLocation& location) const;

    private:
        double m_y;
        double m_x;
    };
}