#include "CCone.h"

#include <cmath>
#include <sstream>
#include <iomanip>

CCone::CCone(const double density, const double baseRadius, const double height)
    : CSolidBody(density)
    , m_baseRadius(baseRadius)
    , m_height(height)
{
}

double CCone::GetBaseRadius() const
{
    return m_baseRadius;
}

double CCone::GetHeight() const
{
    return m_height;
}

double CCone::GetVolume() const
{
    return (1.0 / 3.0) * M_PI * m_baseRadius * m_baseRadius * m_height;
}

std::string CCone::ToString() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Cone:\n";
    oss << "  Density: " << GetDensity() << " kg/m3\n";
    oss << "  Base radius: " << m_baseRadius << " m\n";
    oss << "  Height: " << m_height << " m\n";
    oss << "  Volume: " << GetVolume() << " m3\n";
    oss << "  Mass: " << GetMass() << " kg";
    return oss.str();
}