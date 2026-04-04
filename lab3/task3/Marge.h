#ifndef OOP_MARGE_H
#define OOP_MARGE_H

#include "IActor.h"
#include "Apu.h"

class Marge : public IActor
{
public:
    Marge(Bank& bank, Money initialCash);

    void Act() override;

    const std::string& GetName() const override;
    Money GetCash() const override;

    void ReceiveCash(Money amount) override;

    [[nodiscard]] AccountId GetAccountId() const;

    void SetApu(Apu* apu);

private:
    void BuyGroceries() const;

    static constexpr Money kGroceryCost = 40;

    Bank& m_bank;
    AccountId m_accountId;
    Money m_cash;
    std::string m_name = "Marge";

    Apu* m_apu = nullptr;

};
#endif //OOP_MARGE_H