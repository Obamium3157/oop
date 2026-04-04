#include "Apu.h"

#include <iostream>

Apu::Apu(Bank& bank, const Money initialCash)
    : m_bank(bank)
    , m_accountId(m_bank.OpenAccount())
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

    std::cout << m_name << ": deposited " << m_cash << " ammount of money.\n";
    m_cash = 0;
}

void Apu::PayElectricity() const
{
    if (!m_bank.TrySendMoney(m_accountId, m_burnsAccountId, kElectricityBill))
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

void Apu::ReceiveCash(Money amount)
{
    m_cash += amount;
}

AccountId Apu::GetAccountId() const
{
    return m_accountId;
}

void Apu::SetBurnsAccountId(AccountId burnsAccountId)
{
    m_burnsAccountId = burnsAccountId;
}