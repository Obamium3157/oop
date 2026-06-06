#include "CCanvas.h"

namespace
{
    sf::Color ToSfColor(const uint32_t color)
    {
        return {
            static_cast<uint8_t>((color >> 16) & 0xff),
            static_cast<uint8_t>((color >> 8) & 0xff),
            static_cast<uint8_t>(color & 0xff)
        };
    }

    sf::Vector2f ToSfVector(const CPoint point)
    {
        return {
            static_cast<float>(point.x),
            static_cast<float>(point.y)
        };
    }
}

CCanvas::CCanvas(sf::RenderWindow& window)
    : m_window(window)
{
}

void CCanvas::DrawLine(const CPoint from, const CPoint to, const uint32_t lineColor)
{
    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
    line[0].position = ToSfVector(from);
    line[0].color = ToSfColor(lineColor);
    line[1].position = ToSfVector(to);
    line[1].color = ToSfColor(lineColor);
    m_window.draw(line);
}

void CCanvas::FillPolygon(const std::vector<CPoint>& points, const uint32_t fillColor)
{
    sf::ConvexShape polygon(points.size());
    for (std::size_t i = 0; i < points.size(); ++i)
    {
        polygon.setPoint(i, ToSfVector(points[i]));
    }
    polygon.setFillColor(ToSfColor(fillColor));
    polygon.setOutlineThickness(0.0f);
    m_window.draw(polygon);
}

void CCanvas::DrawCircle(const CPoint center, const double radius, const uint32_t lineColor)
{
    const auto floatRadius = static_cast<float>(radius);
    sf::CircleShape circle(floatRadius);
    circle.setPosition(ToSfVector(center) - sf::Vector2f(floatRadius, floatRadius));
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(ToSfColor(lineColor));
    circle.setOutlineThickness(1.0f);
    m_window.draw(circle);
}

void CCanvas::FillCircle(const CPoint center, const double radius, const uint32_t fillColor)
{
    const auto floatRadius = static_cast<float>(radius);
    sf::CircleShape circle(floatRadius);
    circle.setPosition(ToSfVector(center) - sf::Vector2f(floatRadius, floatRadius));
    circle.setFillColor(ToSfColor(fillColor));
    circle.setOutlineThickness(0.0f);
    m_window.draw(circle);
}
