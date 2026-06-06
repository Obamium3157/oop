#include "CRectangle.h"

#include <iomanip>
#include <sstream>

#include "ColorUtils.h"

CRectangle::CRectangle(const CPoint leftTop, const double width, const double height,
                       const uint32_t outlineColor, const uint32_t fillColor)
    : m_leftTop(leftTop)
    , m_width(width)
    , m_height(height)
    , m_outlineColor(outlineColor)
    , m_fillColor(fillColor)
{
}

double CRectangle::GetArea() const
{
    return m_width * m_height;
}

double CRectangle::GetPerimeter() const
{
    return 2.0 * (m_width + m_height);
}

std::string CRectangle::ToString() const
{
    CPoint rightBottom = GetRightBottom();
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2);
    stream << "Rectangle\n";
    stream << "  Left-top: (" << m_leftTop.x << ", " << m_leftTop.y << ")\n";
    stream << "  Right-bottom: (" << rightBottom.x << ", " << rightBottom.y << ")\n";
    stream << "  Width: " << m_width << "\n";
    stream << "  Height: " << m_height << "\n";
    stream << "  Area: " << GetArea() << "\n";
    stream << "  Perimeter: " << GetPerimeter() << "\n";
    stream << "  Outline: " << FormatColor(m_outlineColor) << "\n";
    stream << "  Fill: " << FormatColor(m_fillColor);
    return stream.str();
}

uint32_t CRectangle::GetOutlineColor() const
{
    return m_outlineColor;
}

uint32_t CRectangle::GetFillColor() const
{
    return m_fillColor;
}

CPoint CRectangle::GetLeftTop() const
{
    return m_leftTop;
}

CPoint CRectangle::GetRightBottom() const
{
    return { m_leftTop.x + m_width, m_leftTop.y + m_height };
}

double CRectangle::GetWidth() const
{
    return m_width;
}
double CRectangle::GetHeight() const
{
    return m_height;
}
