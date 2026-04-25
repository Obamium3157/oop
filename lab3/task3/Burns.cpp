#include "Burns.h"
#include "SimulationContext.h"

#include <iostream>

Burns::Burns(Bank& bank, SimulationContext& context, const Money initialCash)
    : Bankable(bank)
    , m_context(context)
    , m_cash(initialCash)
{
}

void Burns::Act()
{
    PayHomerSalary();
}

void Burns::PayHomerSalary() const
{
    const IActor* homer = m_context.GetActor("Homer");
    if (homer == nullptr)
    {
        return;
    }

    const auto homerAccountId = homer->GetBankAccountId();
    if (!homerAccountId)
    {
        return;
    }

    if (!m_bank.TrySendMoney(m_accountId, *homerAccountId, kHomerSalary))
    {
        std::cout << m_name << ": not enough funds to pay salary to Homer.\n";
        return;
    }

    std::cout << m_name << ": payed " << kHomerSalary << " salary to Homer.\n";
}

const std::string& Burns::GetName() const
{
    return m_name;
}

Money Burns::GetCash() const
{
    return m_cash;
}

void Burns::ReceiveCash(const Money amount)
{
    m_cash += amount;
}