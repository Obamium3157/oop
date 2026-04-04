#include "Lisa.h"
#include "Apu.h"

#include <iostream>

Lisa::Lisa(const Money initialCash)
    : m_cash(initialCash)
{
}

void Lisa::Act()
{
    SpendAtStore();
}

void Lisa::SpendAtStore()
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

    std::cout << m_name << ": spent " << kSpendAmount << " at Apu's.\n";
}

const std::string& Lisa::GetName() const
{
    return m_name;
}

Money Lisa::GetCash() const
{
    return m_cash;
}

void Lisa::ReceiveCash(const Money amount)
{
    m_cash += amount;
}

void Lisa::SetApu(Apu* apu)
{
    m_apu = apu;
}
