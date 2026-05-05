#ifndef OOP_CPARALLELEPIPED_H
#define OOP_CPARALLELEPIPED_H

#include "CSolidBody.h"

class CParallelepiped : public CSolidBody
{
public:
    CParallelepiped(double density, double width, double height, double depth);

    double GetWidth() const;
    double GetHeight() const;
    double GetDepth() const;

    double GetVolume() const override;
    std::string ToString() const override;

private:
    double m_width;
    double m_height;
    double m_depth;
};

#endif //OOP_CPARALLELEPIPED_H
