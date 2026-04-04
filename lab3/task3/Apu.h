#ifndef OOP_APU_H
#define OOP_APU_H

#include "IActor.h"

class Apu : public IActor
{
public:
    Apu(Bank& bank, Money initialCash);

    void Act() override;

    const std::string& GetName() const override;
    Money GetCash() const override;

    void ReceiveCash(Money amount) override;

    [[nodiscard]] AccountId GetAccountId() const;

    void SetBurnsAccountId(AccountId burnsAccountId);

private:
    void DepositCashToAccount();
    void PayElectricity() const;

    static constexpr Money kElectricityBill = 25;

    Bank& m_bank;
    AccountId m_accountId;
    Money m_cash;
    std::string m_name = "Apu";

    AccountId m_burnsAccountId = 0;
};

#endif //OOP_APU_H
