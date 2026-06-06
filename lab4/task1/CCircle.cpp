#include "CCircle.h"

#include <iomanip>
#include <numbers>
#include <sstream>

#include "ColorUtils.h"

CCircle::CCircle(const CPoint center, const double radius, const uint32_t outlineColor, const uint32_t fillColor)
    : m_center(center)
    , m_radius(radius)
    , m_outlineColor(outlineColor)
    , m_fillColor(fillColor)
{
}

double CCircle::GetArea() const
{
    return std::numbers::pi * m_radius * m_radius;
}

double CCircle::GetPerimeter() const
{
    return 2.0 * std::numbers::pi * m_radius;
}

std::string CCircle::ToString() const
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2);
    stream << "Circle\n";
    stream << "  Center: (" << m_center.x << ", " << m_center.y << ")\n";
    stream << "  Radius: " << m_radius << "\n";
    stream << "  Area: " << GetArea() << "\n";
    stream << "  Perimeter: " << GetPerimeter() << "\n";
    stream << "  Outline: " << FormatColor(m_outlineColor) << "\n";
    stream << "  Fill: " << FormatColor(m_fillColor);
    return stream.str();
}

void CCircle::Draw(ICanvas& canvas) const
{
    canvas.FillCircle(m_center, m_radius, m_fillColor);
    canvas.DrawCircle(m_center, m_radius, m_outlineColor);
}

uint32_t CCircle::GetOutlineColor() const
{
    return m_outlineColor;
}

uint32_t CCircle::GetFillColor() const
{
    return m_fillColor;
}

CPoint CCircle::GetCenter() const
{
    return m_center;
}

double CCircle::GetRadius() const
{
    return m_radius;
}