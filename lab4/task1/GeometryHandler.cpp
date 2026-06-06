#include "GeometryHandler.h"

#include <iomanip>

#include "CCircle.h"
#include "CLineSegment.h"
#include "CPoint.h"
#include "CRectangle.h"
#include "CTriangle.h"

#include <iostream>

#include "ColorUtils.h"
#include "FindMaxEx.h"
#include "ICanvas.h"

namespace
{
    uint32_t ParseColor(const std::string& colorString)
    {
        return static_cast<uint32_t>(std::stoul(colorString, nullptr, 16));
    }

    CPoint ReadPoint(std::istream& input)
    {
        CPoint point;
        input >> point.x >> point.y;
        return point;
    }

    std::unique_ptr<IShape> ReadLineSegment(std::istream& input)
    {
        CPoint startPoint = ReadPoint(input);
        CPoint endPoint = ReadPoint(input);
        std::string outlineColorString;
        input >> outlineColorString;
        if (!input)
        {
            throw std::runtime_error("Incomplete line segment definition");
        }
        return std::make_unique<CLineSegment>(
            startPoint, endPoint, ParseColor(outlineColorString));
    }

    std::unique_ptr<IShape> ReadTriangle(std::istream& input)
    {
        CPoint vertex1 = ReadPoint(input);
        CPoint vertex2 = ReadPoint(input);
        CPoint vertex3 = ReadPoint(input);
        std::string outlineColorString;
        std::string fillColorString;
        input >> outlineColorString >> fillColorString;
        if (!input)
        {
            throw std::runtime_error("Incomplete triangle definition");
        }
        return std::make_unique<CTriangle>(
            vertex1, vertex2, vertex3,
            ParseColor(outlineColorString),
            ParseColor(fillColorString));
    }

    std::unique_ptr<IShape> ReadRectangle(std::istream& input)
    {
        CPoint leftTop = ReadPoint(input);
        double width;
        double height;
        input >> width >> height;
        std::string outlineColorString;
        std::string fillColorString;
        input >> outlineColorString >> fillColorString;
        if (!input)
        {
            throw std::runtime_error("Incomplete rectangle definition");
        }
        return std::make_unique<CRectangle>(
            leftTop, width, height,
            ParseColor(outlineColorString),
            ParseColor(fillColorString));
    }

    std::unique_ptr<IShape> ReadCircle(std::istream& input)
    {
        CPoint center = ReadPoint(input);
        double radius;
        input >> radius;
        std::string outlineColorString;
        std::string fillColorString;
        input >> outlineColorString >> fillColorString;
        if (!input)
        {
            throw std::runtime_error("Incomplete circle definition");
        }
        return std::make_unique<CCircle>(
            center, radius,
            ParseColor(outlineColorString),
            ParseColor(fillColorString));
    }

    std::unique_ptr<IShape> ReadShape(const std::string& shapeType, std::istream& input)
    {
        if (shapeType == "line")
        {
            return ReadLineSegment(input);
        }
        if (shapeType == "triangle")
        {
            return ReadTriangle(input);
        }
        if (shapeType == "rectangle")
        {
            return ReadRectangle(input);
        }
        if (shapeType == "circle")
        {
            return ReadCircle(input);
        }
        throw std::invalid_argument("Unknown shape type: " + shapeType);
    }
}

void GeometryHandler::ReadShapes(std::istream& input)
{
    std::string shapeType;
    while (input >> shapeType)
    {
        if (shapeType == "\x1a" || shapeType == "^Z")
        {
            break;
        }
        try
        {
            m_shapes.push_back(ReadShape(shapeType, input));
        }
        catch (const std::exception& exception)
        {
            std::cerr << "Stopped reading: " << exception.what() << "\n";
            break;
        }
    }
}

void GeometryHandler::PrintResults(std::ostream& output) const
{
    if (m_shapes.empty())
    {
        output << "No shapes provided.\n";
        return;
    }

    output << "Shape with maximum area:\n";
    PrintShapeInfo(output, *FindShapeWithMaxArea());
    output << "\n";

    output << "Shape with minimum perimeter:\n";
    PrintShapeInfo(output, *FindShapeWithMinPerimeter());
    output << "\n";
}

void GeometryHandler::Draw(ICanvas& canvas) const
{
    for (const auto& shape : m_shapes)
    {
        shape->Draw(canvas);
    }
}

void GeometryHandler::PrintShapeInfo(std::ostream& output, const IShape& shape)
{
    output << shape.ToString();
}

const IShape* GeometryHandler::FindShapeWithMaxArea() const
{
    const auto it = FindMaxEx(m_shapes.begin(), m_shapes.end(),
    [](const auto& lhs, const auto& rhs)
    noexcept
    {
        return lhs->GetArea() < rhs->GetArea();
    });
    return it != m_shapes.end() ? it->get() : nullptr;
}

const IShape* GeometryHandler::FindShapeWithMinPerimeter() const
{
    const auto it = FindMaxEx(m_shapes.begin(), m_shapes.end(),
    [](const auto& lhs, const auto& rhs)
    noexcept
    {
        return lhs->GetPerimeter() > rhs->GetPerimeter();
    });
    return it != m_shapes.end() ? it->get() : nullptr;
}

/*
rectangle 10.3 20.15 30.7 40.4 ff0000 00ff00
triangle 0 0 0 10 100 50 ffffff bbbbbb
line 10 10 200 200 123321
circle 50 50 12.31 fff000 000fff
*/
