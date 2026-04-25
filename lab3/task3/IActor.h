#ifndef OOP_IACTOR_H
#define OOP_IACTOR_H

#include <optional>

#include "ActorName.h"
#include "Bank.h"

class IActor
{
public:
    virtual ~IActor() = default;

    virtual void Act() = 0;

    virtual const ActorName GetName() const = 0;
    virtual Money GetCash() const = 0;
    virtual void ReceiveCash(Money amount) = 0;

    virtual std::optional<AccountId> GetBankAccountId() const { return std::nullopt; }
};

#endif //OOP_IACTOR_H
