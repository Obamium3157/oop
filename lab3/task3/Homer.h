#ifndef OOP_HOMER_H
#define OOP_HOMER_H

#include "Bankable.h"
#include "IActor.h"

class SimulationContext;

class Homer : public IActor, public Bankable
{
public:
    Homer(Bank& bank, SimulationContext& context, Money initialCash);

    void Act() override;

    const ActorName GetName() const override;
    Money GetCash() const override;
    void ReceiveCash(Money amount) override;

    std::optional<AccountId> GetBankAccountId() const override;

private:
    void PayMarge() const;
    void PayElectricity() const;
    void GiveCashToChild(ActorName childName) const;

    static constexpr Money kMargeAllowance = 50;
    static constexpr Money kElectricityBill = 30;
    static constexpr Money kChildAllowance = 10;

    SimulationContext& m_context;
    Money m_cash;
    ActorName m_name = ActorName::Homer;
};

#endif //OOP_HOMER_H