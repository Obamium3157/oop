#ifndef OOP_GEOMETRYHANDLER_H
#define OOP_GEOMETRYHANDLER_H

#include <iosfwd>
#include <memory>
#include <vector>

#include "IShape.h"

class GeometryHandler
{
public:
    void ReadShapes(std::istream& input);
    void PrintResults(std::ostream& output) const;

private:
    const IShape* FindShapeWithMaxArea() const;
    const IShape* FindShapeWithMinPerimeter() const;

    static void PrintShapeInfo(std::ostream& output, const IShape& shape);

    std::vector<std::unique_ptr<IShape>> m_shapes;
};

#endif //OOP_GEOMETRYHANDLER_H
