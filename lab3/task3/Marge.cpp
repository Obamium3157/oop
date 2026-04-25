#include "Marge.h"
#include "SimulationContext.h"

#include <iostream>

Marge::Marge(Bank& bank, SimulationContext& context, const Money initialCash)
    : Bankable(bank)
    , m_context(context)
    , m_cash(initialCash)
{
}

void Marge::Act()
{
    BuyGroceries();
}

void Marge::BuyGroceries() const
{
    const IActor* apu = m_context.GetActor("Apu");
    if (apu == nullptr)
    {
        return;
    }

    const auto apuAccountId = apu->GetBankAccountId();
    if (!apuAccountId)
    {
        std::cout << m_name << ": Apu has no bank account.\n";
        return;
    }

    if (!m_bank.TrySendMoney(m_accountId, *apuAccountId, kGroceryCost))
    {
        std::cout << m_name << ": not enough funds to buy groceries at Apu's.\n";
        return;
    }

    std::cout << m_name << ": payed " << kGroceryCost << " for groceries at Apu's.\n";
}

const std::string& Marge::GetName() const
{
    return m_name;
}

Money Marge::GetCash() const
{
    return m_cash;
}

void Marge::ReceiveCash(const Money amount)
{
    m_cash += amount;
}