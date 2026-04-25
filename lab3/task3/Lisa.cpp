#include "Lisa.h"
#include "Apu.h"

#include <iostream>

Lisa::Lisa(SimulationContext& context, const Money initialCash)
    : m_context(context)
    , m_cash(initialCash)
{
}

void Lisa::Act()
{
    SpendAtStore();
}

void Lisa::SpendAtStore()
{
    if (m_cash < kSpendAmount)
    {
        std::cout << m_name << ": not enough cash to purchase at Apu's.\n";
        return;
    }

    IActor* apu = m_context.GetActor(ActorName::Apu);
    if (apu == nullptr)
    {
        return;
    }

    m_cash -= kSpendAmount;
    apu->ReceiveCash(kSpendAmount);

    std::cout << m_name << ": spent " << kSpendAmount << " at Apu's.\n";
}

const ActorName Lisa::GetName() const
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