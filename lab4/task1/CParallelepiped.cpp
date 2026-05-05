#include "CParallelepiped.h"

#include <sstream>
#include <iomanip>

CParallelepiped::CParallelepiped(const double density, const double width, const double height, const double depth)
    : CSolidBody(density)
    , m_width(width)
    , m_height(height)
    , m_depth(depth)
{
}

double CParallelepiped::GetWidth() const
{
    return m_width;
}

double CParallelepiped::GetHeight() const
{
    return m_height;
}

double CParallelepiped::GetDepth() const
{
    return m_depth;
}

double CParallelepiped::GetVolume() const
{
    return m_width * m_height * m_depth;
}

std::string CParallelepiped::ToString() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Parallelepiped:\n";
    oss << "  Density: " << GetDensity() << " kg/m3\n";
    oss << "  Width: " << m_width << " m\n";
    oss << "  Height: " << m_height << " m\n";
    oss << "  Depth: " << m_depth << " m\n";
    oss << "  Volume: " << GetVolume() << " m3\n";
    oss << "  Mass: " << GetMass() << " kg";
    return oss.str();
}