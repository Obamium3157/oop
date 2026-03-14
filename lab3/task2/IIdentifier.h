#ifndef OOP_IIDENTIFIER_H
#define OOP_IIDENTIFIER_H


class IIdentifier
{
public:
    virtual ~IIdentifier() = default;
    virtual double GetValue() const = 0;
};


#endif //OOP_IIDENTIFIER_H