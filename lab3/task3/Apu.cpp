#include "Apu.h"
#include "SimulationContext.h"

#include <iostream>

Apu::Apu(Bank& bank, SimulationContext& context, const Money initialCash)
    : Bankable(bank)
    , m_context(context)
    , m_cash(initialCash)
{
}

void Apu::Act()
{
    DepositCashToAccount();
    PayElectricity();
}

void Apu::DepositCashToAccount()
{
    if (m_cash == 0)
    {
        return;
    }

    try
    {
        m_bank.DepositMoney(m_accountId, m_cash);
    }
    catch (const BankOperationError&)
    {
        std::cout << m_name << ": error depositing money.\n";
        return;
    }

    std::cout << m_name << ": deposited " << m_cash << " amount of money.\n";
    m_cash = 0;
}

void Apu::PayElectricity() const
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
        std::cout << m_name << ": not enough money to pay for electricity.\n";
        return;
    }

    std::cout << m_name << ": payed " << kElectricityBill << " for electricity.\n";
}

const std::string& Apu::GetName() const
{
    return m_name;
}
Money Apu::GetCash() const
{
    return m_cash;
}
void Apu::ReceiveCash(const Money amount)
{
    m_cash += amount;
}