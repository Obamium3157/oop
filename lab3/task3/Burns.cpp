#include "Burns.h"

#include <iostream>

Burns::Burns(Bank& bank, const Money initialCash)
    : m_bank(bank)
      , m_accountId(m_bank.OpenAccount())
      , m_cash(initialCash)
{
}

void Burns::Act()
{
    PayHomerSalary();
}

void Burns::PayHomerSalary() const
{
    if (!m_bank.TrySendMoney(m_accountId, m_homerAccountId, kHomerSalary))
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

AccountId Burns::GetAccountId() const
{
    return m_accountId;
}

void Burns::SetHomerAccountId(const AccountId homerAccountId)
{
    m_homerAccountId = homerAccountId;
}
