#ifndef OOP_BART_H
#define OOP_BART_H

#include "IActor.h"
#include "Apu.h"

class Bart : public IActor
{
public:
    Bart(Money initialCash);

    void Act() override;

    const std::string& GetName() const override;
    Money GetCash() const override;

    void ReceiveCash(Money amount) override;

    void SetApu(Apu* apu);

private:
    void SpendAtStore();

    static constexpr Money kSpendAmount = 5;

    Money m_cash;
    std::string m_name = "Bart";

    Apu* m_apu = nullptr;
};

#endif //OOP_BART_H
