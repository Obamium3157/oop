#include "Bart.h"
#include "SimulationContext.h"

#include <iostream>

Bart::Bart(SimulationContext& context, const Money initialCash)
    : m_context(context)
    , m_cash(initialCash)
{
}

void Bart::Act()
{
    SpendAtStore();
}

void Bart::SpendAtStore()
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

    std::cout << m_name << ": spent " << kSpendAmount << " amount of cash at Apu's.\n";
}

const ActorName Bart::GetName() const
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