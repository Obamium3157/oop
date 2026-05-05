#include "CCylinder.h"

#include <cmath>
#include <sstream>
#include <iomanip>

CCylinder::CCylinder(const double density, const double baseRadius, const double height)
    : CSolidBody(density)
    , m_baseRadius(baseRadius)
    , m_height(height)
{
}

double CCylinder::GetBaseRadius() const
{
    return m_baseRadius;
}

double CCylinder::GetHeight() const
{
    return m_height;
}

double CCylinder::GetVolume() const
{
    return M_PI * m_baseRadius * m_baseRadius * m_height;
}

std::string CCylinder::ToString() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Cylinder:\n";
    oss << "  Density: " << GetDensity() << " kg/m3\n";
    oss << "  Base radius: " << m_baseRadius << " m\n";
    oss << "  Height: " << m_height << " m\n";
    oss << "  Volume: " << GetVolume() << " m3\n";
    oss << "  Mass: " << GetMass() << " kg";
    return oss.str();
}