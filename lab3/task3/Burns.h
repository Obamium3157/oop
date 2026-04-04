#ifndef OOP_BURNS_H
#define OOP_BURNS_H

#include "IActor.h"

class Burns : public IActor
{
public:
    Burns(Bank& bank, Money initialCash);

    void Act() override;

    const std::string& GetName() const override;
    Money GetCash() const override;

    void ReceiveCash(Money amount) override;

    [[nodiscard]] AccountId GetAccountId() const;

    void SetHomerAccountId(AccountId homerAccountId);

private:
    void PayHomerSalary() const;

    static constexpr Money kHomerSalary = 100;

    Bank& m_bank;
    AccountId m_accountId;
    Money m_cash;
    std::string m_name = "Burns";

    AccountId m_homerAccountId = 0;
};

#endif //OOP_BURNS_H
