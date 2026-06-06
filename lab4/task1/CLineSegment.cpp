#include "CLineSegment.h"

#include <iomanip>
#include <cmath>

#include "ColorUtils.h"

CLineSegment::CLineSegment(const CPoint startPoint, const CPoint endPoint, const uint32_t outlineColor)
    : m_startPoint(startPoint)
    , m_endPoint(endPoint)
    , m_outlineColor(outlineColor)
{
}

double CLineSegment::GetArea() const
{
    return 0.0;
}


double CLineSegment::GetPerimeter() const
{
    const double dx = m_endPoint.x - m_startPoint.x;
    const double dy = m_endPoint.y - m_startPoint.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::string CLineSegment::ToString() const
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2);
    stream << "Line Segment\n";
    stream << "  Start: (" << m_startPoint.x << ", " << m_startPoint.y << ")\n";
    stream << "  End: (" << m_endPoint.x << ", " << m_endPoint.y << ")\n";
    stream << "  Perimeter: " << GetPerimeter() << "\n";
    stream << "  Outline: " << FormatColor(m_outlineColor);
    return stream.str();
}

uint32_t CLineSegment::GetOutlineColor() const
{
    return m_outlineColor;
}

CPoint CLineSegment::GetStartPoint() const
{
    return m_startPoint;
}

CPoint CLineSegment::GetEndPoint() const
{
    return m_endPoint;
}
