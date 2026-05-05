#ifndef OOP_CCYLINDER_H
#define OOP_CCYLINDER_H
#include "CSolidBody.h"

class CCylinder : public CSolidBody
{
public:
    CCylinder(double density, double baseRadius, double height);

    double GetBaseRadius() const;
    double GetHeight() const;

    double GetVolume() const override;
    std::string ToString() const override;

private:
    double m_baseRadius;
    double m_height;
};

#endif //OOP_CCYLINDER_H
