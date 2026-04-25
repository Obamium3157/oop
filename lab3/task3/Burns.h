#ifndef OOP_BURNS_H
#define OOP_BURNS_H

#include "Bankable.h"
#include "IActor.h"
#include "SimulationContext.h"


class Burns : public IActor, public Bankable
{
public:
    Burns(Bank& bank, SimulationContext& context, Money initialCash);

    void Act() override;

    const ActorName GetName() const override;
    Money GetCash() const override;
    void ReceiveCash(Money amount) override;

    std::optional<AccountId> GetBankAccountId() const override;

private:
    void PayHomerSalary() const;

    static constexpr Money kHomerSalary = 100;

    SimulationContext& m_context;
    Money m_cash;
    ActorName m_name = ActorName::Burns;
};

#endif //OOP_BURNS_H