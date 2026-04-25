#ifndef OOP_MARGE_H
#define OOP_MARGE_H

#include "Bankable.h"
#include "IActor.h"
#include "SimulationContext.h"

class Marge : public IActor, public Bankable
{
public:
    Marge(Bank& bank, SimulationContext& context, Money initialCash);

    void Act() override;

    const std::string& GetName() const override;
    Money GetCash() const override;
    void ReceiveCash(Money amount) override;

    std::optional<AccountId> GetBankAccountId() const override { return Bankable::GetBankAccountId(); }

private:
    void BuyGroceries() const;

    static constexpr Money kGroceryCost = 40;

    SimulationContext& m_context;
    Money m_cash;
    std::string m_name = "Marge";
};
#endif //OOP_MARGE_H