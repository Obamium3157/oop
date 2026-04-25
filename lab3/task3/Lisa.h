#ifndef OOP_LISA_H
#define OOP_LISA_H

#include "IActor.h"
#include "SimulationContext.h"

class Lisa : public IActor
{
public:
    Lisa(SimulationContext& context, Money initialCash);

    void Act() override;

    const std::string& GetName() const override;
    Money GetCash() const override;
    void ReceiveCash(Money amount) override;

private:
    void SpendAtStore();

    static constexpr Money kSpendAmount = 5;

    SimulationContext& m_context;
    Money m_cash;
    std::string m_name = "Lisa";
};

#endif //OOP_LISA_H