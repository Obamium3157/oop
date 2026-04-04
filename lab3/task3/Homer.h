#ifndef OOP_HOMER_H
#define OOP_HOMER_H

#include "IActor.h"
#include "Marge.h"
#include "Bart.h"
#include "Lisa.h"

class Homer : public IActor
{
public:
    Homer(Bank& bank, Money initialCash);

    void Act() override;

    const std::string& GetName() const override;
    Money GetCash() const override;

    void ReceiveCash(Money amount) override;

    [[nodiscard]] AccountId GetAccountId() const;

    void SetMarge(Marge* marge);
    void SetBart(Bart* bart);
    void SetLisa(Lisa* lisa);
    void SetBurnsAccountId(AccountId burnsAccountId);

private:
    void PayMarge() const;
    void PayElectricity() const;
    void GiveCashToChild(IActor* child, const std::string& childName) const;

    static constexpr Money kMargeAllowance = 50;
    static constexpr Money kElectricityBill = 30;
    static constexpr Money kChildAllowance = 10;

    Bank& m_bank;
    AccountId m_accountId;
    Money m_cash;
    std::string m_name = "Homer";

    Marge* m_marge = nullptr;
    Bart* m_bart = nullptr;
    Lisa* m_lisa = nullptr;
    AccountId m_burnsAccountId = 0;
};
#endif //OOP_HOMER_H