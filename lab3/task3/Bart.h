#ifndef OOP_BART_H
#define OOP_BART_H

#include "IActor.h"

class SimulationContext;

class Bart : public IActor
{
public:
    Bart(SimulationContext& context, Money initialCash);

    void Act() override;

    const ActorName GetName() const override;
    Money GetCash() const override;
    void ReceiveCash(Money amount) override;

private:
    void SpendAtStore();

    static constexpr Money kSpendAmount = 5;

    SimulationContext& m_context;
    Money m_cash;
    ActorName m_name = ActorName::Bart;
};

#endif //OOP_BART_H