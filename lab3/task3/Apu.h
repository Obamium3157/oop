#ifndef OOP_APU_H
#define OOP_APU_H

#include "Bankable.h"
#include "IActor.h"
#include "SimulationContext.h"

class Apu : public IActor, public Bankable
{
public:
    Apu(Bank& bank, SimulationContext& context, Money initialCash);

    void Act() override;

    const std::string& GetName() const override;
    Money GetCash() const override;
    void ReceiveCash(Money amount) override;

    std::optional<AccountId> GetBankAccountId() const override { return Bankable::GetBankAccountId(); }

private:
    void DepositCashToAccount();
    void PayElectricity() const;

    static constexpr Money kElectricityBill = 25;

    SimulationContext& m_context;
    Money m_cash;
    std::string m_name = "Apu";
};

#endif //OOP_APU_H