#ifndef OOP_MARGE_H
#define OOP_MARGE_H

#include "Bankable.h"
#include "IActor.h"

class SimulationContext;

class Marge : public IActor, public Bankable
{
public:
    Marge(Bank& bank, SimulationContext& context, Money initialCash);

    void Act() override;

    const ActorName GetName() const override;
    Money GetCash() const override;
    void ReceiveCash(Money amount) override;

    std::optional<AccountId> GetBankAccountId() const override;

private:
    void BuyGroceries() const;

    static constexpr Money kGroceryCost = 40;

    SimulationContext& m_context;
    Money m_cash;
    ActorName m_name = ActorName::Marge;
};
#endif //OOP_MARGE_H