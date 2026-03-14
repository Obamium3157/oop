#ifndef OOP_IIDENTIFIER_H
#define OOP_IIDENTIFIER_H


class IValue
{
public:
    virtual ~IValue() = default;
    virtual double GetValue() const = 0;
};

#endif //OOP_IIDENTIFIER_H