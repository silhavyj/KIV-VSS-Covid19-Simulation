#pragma once

#include <iostream>
#include <utility>

namespace kiv_vss
{
    class CLocation
    {
    public:
        explicit CLocation(const double y, const double x);
        CLocation(const CLocation& location);
        ~CLocation() = default;

        void operator=(const CLocation& location);
        [[nodiscard]] double operator-(const CLocation& location) const;
        [[nodiscard]] bool operator==(const CLocation& location) const;
        [[nodiscard]] bool operator!=(const CLocation& location) const;

        [[nodiscard]] double Get_Y() const;
        [[nodiscard]] double Get_X() const;
        [[nodiscard]] std::pair<double, double> Get_Coordinates() const;

        void Move_Toward(const CLocation& location, const double distance);

        friend std::ostream& operator<<(std::ostream& out, const CLocation& location);
        friend std::string to_string(const CLocation& location);

    private:
        [[nodiscard]] std::pair<double, double> Get_Coordinates_Diff(const CLocation& location) const;

    private:
        double m_y;
        double m_x;
    };
}