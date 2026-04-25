#ifndef OOP_BANKABLE_H
#define OOP_BANKABLE_H

#include <optional>
#include "Bank.h"

class Bankable
{
public:
    explicit Bankable(Bank& bank);

    std::optional<AccountId> GetBankAccountId() const;

protected:
    Bank& m_bank;
    AccountId m_accountId;
};

#endif //OOP_BANKABLE_H