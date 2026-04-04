#include "Bart.h"
#include "Apu.h"

#include <iostream>

Bart::Bart(const Money initialCash)
    : m_cash(initialCash)
{
}

void Bart::Act()
{
    SpendAtStore();
}

void Bart::SpendAtStore()
{
    if (m_apu == nullptr)
    {
        return;
    }

    if (m_cash < kSpendAmount)
    {
        std::cout << m_name << ": not enough cash to purchase at Apu's.\n";
        return;
    }

    m_cash -= kSpendAmount;
    m_apu->ReceiveCash(kSpendAmount);

    std::cout << m_name << ": spent " << kSpendAmount << " amount of cash at Apu's.\n";
}

const std::string& Bart::GetName() const
{
    return m_name;
}

Money Bart::GetCash() const
{
    return m_cash;
}

void Bart::ReceiveCash(const Money amount)
{
    m_cash += amount;
}

void Bart::SetApu(Apu* apu)
{
    m_apu = apu;
}