#include "Homer.h"
#include "SimulationContext.h"

#include <iostream>

Homer::Homer(Bank& bank, SimulationContext& context, const Money initialCash)
    : Bankable(bank)
    , m_context(context)
    , m_cash(initialCash)
{
}

void Homer::Act()
{
    PayMarge();
    PayElectricity();
    GiveCashToChild("Bart");
    GiveCashToChild("Lisa");
}

void Homer::PayMarge() const
{
    const IActor* marge = m_context.GetActor("Marge");
    if (marge == nullptr)
    {
        return;
    }

    const auto margeAccountId = marge->GetBankAccountId();
    if (!margeAccountId)
    {
        return;
    }

    if (!m_bank.TrySendMoney(m_accountId, *margeAccountId, kMargeAllowance))
    {
        std::cout << m_name << ": not enough money to pay to Marge.\n";
        return;
    }

    std::cout << m_name << ": transferred " << kMargeAllowance << " to Marge.\n";
}

void Homer::PayElectricity() const
{
    const IActor* burns = m_context.GetActor("Burns");
    if (burns == nullptr)
    {
        return;
    }

    const auto burnsAccountId = burns->GetBankAccountId();
    if (!burnsAccountId)
    {
        return;
    }

    if (!m_bank.TrySendMoney(m_accountId, *burnsAccountId, kElectricityBill))
    {
        std::cout << m_name << ": not enough funds to pay for electricity.\n";
        return;
    }

    std::cout << m_name << ": payed " << kElectricityBill << " for electricity.\n";
}

void Homer::GiveCashToChild(const std::string& childName) const
{
    IActor* child = m_context.GetActor(childName);
    if (child == nullptr)
    {
        return;
    }

    if (!m_bank.TryWithdrawMoney(m_accountId, kChildAllowance))
    {
        std::cout << m_name << ": not enough funds for cash withdrawal to " << childName << ".\n";
        return;
    }

    child->ReceiveCash(kChildAllowance);

    std::cout << m_name << ": gave " << kChildAllowance << " amount of cash to " << childName << ".\n";
}

const std::string& Homer::GetName() const
{
    return m_name;
}

Money Homer::GetCash() const
{
    return m_cash;
}

void Homer::ReceiveCash(const Money amount)
{
    m_cash += amount;
}