#ifndef OOP_LISA_H
#define OOP_LISA_H

#include "IActor.h"
#include "Apu.h"

class Lisa : public IActor
{
public:
    Lisa(Money initialCash);

    void Act() override;

    const std::string& GetName() const override;
    Money GetCash() const override;

    void ReceiveCash(Money amount) override;

    void SetApu(Apu* apu);

private:
    void SpendAtStore();

    static constexpr Money kSpendAmount = 5;

    Money m_cash;
    std::string m_name = "Lisa";

    Apu* m_apu = nullptr;
};


#endif //OOP_LISA_H
