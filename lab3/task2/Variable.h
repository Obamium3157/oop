#ifndef OOP_VARIABLE_H
#define OOP_VARIABLE_H
#include <limits>

#include "IValue.h"


class Variable : public IValue
{
public:
    double GetValue() const override;
    void SetValue(double value);

private:
    double m_value = std::numeric_limits<double>::quiet_NaN();
};


#endif //OOP_VARIABLE_H