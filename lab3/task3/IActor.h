#ifndef OOP_IACTOR_H
#define OOP_IACTOR_H

#include <string>
#include "Bank.h"

class IActor
{
public:
    virtual ~IActor() = default;

    virtual void Act() = 0;

    virtual const std::string& GetName() const = 0;
    virtual Money GetCash() const = 0;

    virtual void ReceiveCash(Money amount) = 0;
};


#endif //OOP_IACTOR_H
