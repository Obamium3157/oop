#include "Marge.h"
#include "Apu.h"

#include <iostream>

Marge::Marge(Bank& bank, const Money initialCash)
    : m_bank(bank)
    , m_accountId(m_bank.OpenAccount())
    , m_cash(initialCash)
{
}

void Marge::Act()
{
    BuyGroceries();
}

void Marge::BuyGroceries() const
{
    if (m_apu == nullptr)
    {
        return;
    }

    if (!m_bank.TrySendMoney(m_accountId, m_apu->GetAccountId(), kGroceryCost))
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

AccountId Marge::GetAccountId() const
{
    return m_accountId;
}

void Marge::SetApu(Apu* apu)
{
    m_apu = apu;
}