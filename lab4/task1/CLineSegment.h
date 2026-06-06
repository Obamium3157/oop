#ifndef OOP_CLINESEGMENT_H
#define OOP_CLINESEGMENT_H

#include <cstdint>
#include <string>

#include "IShape.h"
#include "CPoint.h"

class CLineSegment : public IShape
{
public:
    CLineSegment(CPoint startPoint, CPoint endPoint, uint32_t outlineColor);

    double GetArea() const override;
    double GetPerimeter() const override;
    std::string ToString() const override;
    uint32_t GetOutlineColor() const override;

    CPoint GetStartPoint() const;
    CPoint GetEndPoint() const;

private:
    CPoint m_startPoint;
    CPoint m_endPoint;
    uint32_t m_outlineColor;
};

#endif //OOP_CLINESEGMENT_H
