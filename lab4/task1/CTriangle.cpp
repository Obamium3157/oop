#include "CTriangle.h"

#include <cmath>
#include <iomanip>

#include "ColorUtils.h"

namespace
{
    double CalculateDistance(const CPoint from, const CPoint to)
    {
        const double dx = to.x - from.x;
        const double dy = to.y - from.y;

        return std::sqrt(dx * dx + dy * dy);
    }
}

CTriangle::CTriangle(const CPoint vertex1, const CPoint vertex2, const CPoint vertex3,
                     const uint32_t outlineColor, const uint32_t fillColor)
    : m_vertex1(vertex1)
    , m_vertex2(vertex2)
    , m_vertex3(vertex3)
    , m_outlineColor(outlineColor)
    , m_fillColor(fillColor)
{
}

double CTriangle::GetArea() const
{
    return 0.5 * std::abs(
        (m_vertex2.x - m_vertex1.x) * (m_vertex3.y - m_vertex1.y) -
        (m_vertex3.x - m_vertex1.x) * (m_vertex2.y - m_vertex1.y));
}

double CTriangle::GetPerimeter() const
{
    return CalculateDistance(m_vertex1, m_vertex2)
         + CalculateDistance(m_vertex2, m_vertex3)
         + CalculateDistance(m_vertex3, m_vertex1);
}

std::string CTriangle::ToString() const
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2);
    stream << "Triangle\n";
    stream << "  Vertex 1: (" << m_vertex1.x << ", " << m_vertex1.y << ")\n";
    stream << "  Vertex 2: (" << m_vertex2.x << ", " << m_vertex2.y << ")\n";
    stream << "  Vertex 3: (" << m_vertex3.x << ", " << m_vertex3.y << ")\n";
    stream << "  Area: " << GetArea() << "\n";
    stream << "  Perimeter: " << GetPerimeter() << "\n";
    stream << "  Outline: " << FormatColor(m_outlineColor) << "\n";
    stream << "  Fill: " << FormatColor(m_fillColor);
    return stream.str();
}

uint32_t CTriangle::GetOutlineColor() const
{
    return m_outlineColor;
}

uint32_t CTriangle::GetFillColor() const
{
    return m_fillColor;
}

CPoint CTriangle::GetVertex1() const
{
    return m_vertex1;
}

CPoint CTriangle::GetVertex2() const
{
    return m_vertex2;
}

CPoint CTriangle::GetVertex3() const
{
    return m_vertex3;
}