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
        explicit CLocation(double y, double x) noexcept;
        CLocation(const CLocation& location) = default;
        ~CLocation() = default;

        [[nodiscard]] CLocation& operator=(const CLocation& location) = default;
        [[nodiscard]] double operator-(const CLocation& location) const;
        [[nodiscard]] bool operator==(const CLocation& location) const;
        [[nodiscard]] bool operator!=(const CLocation& location) const;

        [[nodiscard]] double Get_Y() const noexcept;
        [[nodiscard]] double Get_X() const noexcept;
        [[nodiscard]] Point Get_Coordinates() const noexcept;

        void Move_Toward(const CLocation& location, double distance);
        static CLocation Generate_Random_In_Square_Location(double min, double max);

        friend std::ostream& operator<<(std::ostream& out, const CLocation& location);
        friend std::string to_string(const CLocation& location);

    private:
        [[nodiscard]] Point Subtract_Coordinates(const CLocation& location) const noexcept;

    private:
        double m_y;
        double m_x;
    };
}