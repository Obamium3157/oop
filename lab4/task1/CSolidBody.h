#ifndef OOP_CSOLIDBODY_H
#define OOP_CSOLIDBODY_H

#include "CBody.h"

class CSolidBody : public CBody
{
public:
    explicit CSolidBody(double density);

    double GetDensity() const final;

private:
    double m_density;
};

#endif //OOP_CSOLIDBODY_H
