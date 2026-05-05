#ifndef OOP_CCONE_H
#define OOP_CCONE_H

#include "CSolidBody.h"

class CCone : public CSolidBody
{
public:
    CCone(double density, double baseRadius, double height);

    double GetBaseRadius() const;
    double GetHeight() const;

    double GetVolume() const override;
    std::string ToString() const override;

private:
    double m_baseRadius;
    double m_height;
};

#endif //OOP_CCONE_H
