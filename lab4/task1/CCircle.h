#ifndef OOP_CCIRLE_H
#define OOP_CCIRLE_H

#include <string>

#include "CPoint.h"
#include "ISolidShape.h"

class CCircle : public ISolidShape
{
public:
    CCircle(CPoint center, double radius, uint32_t outlineColor, uint32_t fillColor);

    double GetArea() const override;
    double GetPerimeter() const override;
    std::string ToString() const override;
    uint32_t GetOutlineColor() const override;
    uint32_t GetFillColor() const override;

    CPoint GetCenter() const;
    double GetRadius() const;

private:
    CPoint m_center;
    double m_radius;
    uint32_t m_outlineColor;
    uint32_t m_fillColor;
};


#endif //OOP_CCIRLE_H
