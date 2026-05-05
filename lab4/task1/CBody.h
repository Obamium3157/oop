#ifndef OOP_CBODY_H
#define OOP_CBODY_H

#include <string>

class CBody
{
public:
    virtual ~CBody() = default;

    double GetMass() const;

    virtual double GetVolume() const = 0;
    virtual double GetDensity() const = 0;
    virtual std::string ToString() const = 0;

    virtual bool ContainsBody(const CBody* body) const;
};

#endif //OOP_CBODY_H
