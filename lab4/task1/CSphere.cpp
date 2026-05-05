#include "CSphere.h"

#include <cmath>
#include <sstream>
#include <iomanip>

CSphere::CSphere(const double density, const double radius)
    : CSolidBody(density)
    , m_radius(radius)
{
}

double CSphere::GetRadius() const
{
    return m_radius;
}

double CSphere::GetVolume() const
{
    return (4.0 / 3.0) * M_PI * m_radius * m_radius * m_radius;
}

std::string CSphere::ToString() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Sphere:\n";
    oss << "  Density: " << GetDensity() << " kg/m3\n";
    oss << "  Radius: " << m_radius << " m\n";
    oss << "  Volume: " << GetVolume() << " m3\n";
    oss << "  Mass: " << GetMass() << " kg\n";
    return oss.str();
}
