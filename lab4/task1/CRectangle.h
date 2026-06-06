#ifndef OOP_CRECTANGLE_H
#define OOP_CRECTANGLE_H

#include <string>

#include "CPoint.h"
#include "ISolidShape.h"

class CRectangle : public ISolidShape
{
public:
    CRectangle(CPoint leftTop, double width, double height,
               uint32_t outlineColor, uint32_t fillColor);

    double GetArea() const override;
    double GetPerimeter() const override;
    std::string ToString() const override;
    uint32_t GetOutlineColor() const override;
    uint32_t GetFillColor() const override;
    void Draw(ICanvas& canvas) const override;

    CPoint GetLeftTop() const;
    CPoint GetRightBottom() const;
    double GetWidth() const;
    double GetHeight() const;

private:
    CPoint m_leftTop;
    double m_width;
    double m_height;
    uint32_t m_outlineColor;
    uint32_t m_fillColor;
};

#endif //OOP_CRECTANGLE_H
