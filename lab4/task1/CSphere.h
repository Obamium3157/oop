#ifndef OOP_CSPHERE_H
#define OOP_CSPHERE_H

#include "CSolidBody.h"

class CSphere : public CSolidBody
{
public:
    CSphere(double density, double radius);

    double GetRadius() const;

    double GetVolume() const override;
    std::string ToString() const override;

private:
    double m_radius;
};

#endif //OOP_CSPHERE_H
