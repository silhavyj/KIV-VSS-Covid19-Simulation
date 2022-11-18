#include <cmath>
#include <sstream>

#include "utils.h"
#include "location.h"

namespace kiv_vss
{
    CLocation::CLocation(double y, double  x)
        : m_y(y),
          m_x(x)
    {

    }

    double CLocation::operator-(const CLocation& location) const
    {
        const auto [dy, dx] = Subtract_Coordinates(location);
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

    CLocation::Point CLocation::Get_Coordinates() const
    {
        return { Get_Y(), Get_X() };
    }

    void CLocation::Move_Toward(const CLocation& location, double distance)
    {
        if (((*this) - location) < distance)
        {
            m_y = location.m_y;
            m_x = location.m_x;
        }
        else
        {
            const auto [dy, dx] = Subtract_Coordinates(location);
            const double theta = std::atan2(dy, dx);

            m_x += distance * std::cos(theta);
            m_y += distance * std::sin(theta);
        }
    }

    inline CLocation::Point CLocation::Subtract_Coordinates(const CLocation& location) const
    {
        return { location.m_y - m_y, location.m_x - m_x };
    }

    CLocation CLocation::Generate_Random_In_Square_Location(double min, double max)
    {
        const auto Get_Random_Axis = [&]()
        {
            return utils::Random<std::uniform_real_distribution<double>>(min, max);
        };

        const double y = Get_Random_Axis();
        const double x = Get_Random_Axis();

        return CLocation(y, x);
    }

    std::ostream& operator<<(std::ostream& out, const CLocation& location)
    {
        const auto [y, x] = location.Get_Coordinates();
        out << "[" << y << "; " << x << "]";
        return out;
    }

    std::string to_string(const CLocation& location)
    {
        std::ostringstream ss;
        ss << location;
        return ss.str();
    }
}