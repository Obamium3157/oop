#ifndef OOP_HOMER_H
#define OOP_HOMER_H

#include "Bankable.h"
#include "IActor.h"
#include "SimulationContext.h"

class Homer : public IActor, public Bankable
{
public:
    Homer(Bank& bank, SimulationContext& context, Money initialCash);

    void Act() override;

    const std::string& GetName() const override;
    Money GetCash() const override;
    void ReceiveCash(Money amount) override;

    std::optional<AccountId> GetBankAccountId() const override { return Bankable::GetBankAccountId(); }

private:
    void PayMarge() const;
    void PayElectricity() const;
    void GiveCashToChild(const std::string& childName) const;

    static constexpr Money kMargeAllowance = 50;
    static constexpr Money kElectricityBill = 30;
    static constexpr Money kChildAllowance = 10;

    SimulationContext& m_context;
    Money m_cash;
    std::string m_name = "Homer";
};

#endif //OOP_HOMER_H