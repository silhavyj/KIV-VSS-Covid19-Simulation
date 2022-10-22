#include <cmath>
#include <sstream>

#include "location.h"

namespace kiv_vss
{
    CLocation::CLocation(const double y, const double x)
        : m_y(y),
          m_x(x)
    {

    }

    CLocation::CLocation(const CLocation& location)
        : m_y(location.m_y),
          m_x(location.m_x)
    {

    }

    void CLocation::operator=(const CLocation& location)
    {
        m_y = location.m_y;
        m_x = location.m_x;
    }

    inline std::pair<double, double> CLocation::Get_Coordinates_Diff(const CLocation& location) const
    {
        return { location.m_y - m_y, location.m_x - m_x };
    }

    double CLocation::operator-(const CLocation& location) const
    {
        const auto [dy, dx] = Get_Coordinates_Diff(location);
        return std::sqrt((dy * dy) + (dx * dx));
    }

    bool CLocation::operator==(const CLocation& location) const
    {
        static constexpr double EPSILON = 0.001;
        return (*this) - location < EPSILON;
    }

    bool CLocation::operator!=(const CLocation& location) const
    {
        return !((*this) == location);
    }

    double CLocation::Get_Y() const
    {
        return m_y;
    }

    double CLocation::Get_X() const
    {
        return m_x;
    }

    std::pair<double, double> CLocation::Get_Coordinates() const
    {
        return { Get_Y(), Get_X() };
    }

    void CLocation::Move_Toward(const CLocation& location, const double distance)
    {
        if (((*this) - location) < distance)
        {
            m_y = location.m_y;
            m_x = location.m_x;
        }
        else
        {
            const auto [dy, dx] = Get_Coordinates_Diff(location);
            const double theta = std::atan2(dy, dx);

            m_x += distance * std::cos(theta);
            m_y += distance * std::sin(theta);
        }
    }

    std::ostream& operator<<(std::ostream& out, const CLocation& location)
    {
        const auto [y, x] = location.Get_Coordinates();
        out << "[" << y << "; " << x << "]";
        return out;
    }

    inline std::string to_string(const CLocation& location)
    {
        std::ostringstream ss;
        ss << location;
        return ss.str();
    }
}