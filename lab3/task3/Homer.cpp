#include "Homer.h"
#include "Marge.h"
#include "Bart.h"
#include "Lisa.h"

#include <iostream>

Homer::Homer(Bank& bank, const Money initialCash)
    : m_bank(bank)
    , m_accountId(m_bank.OpenAccount())
    , m_cash(initialCash)
{
}

void Homer::Act()
{
    PayMarge();
    PayElectricity();
    GiveCashToChild(m_bart, "Bart");
    GiveCashToChild(m_lisa, "Lisa");
}

void Homer::PayMarge() const
{
    if (m_marge == nullptr)
    {
        return;
    }

    if (!m_bank.TrySendMoney(m_accountId, m_marge->GetAccountId(), kMargeAllowance))
    {
        std::cout << m_name << ": not enough money to pay to Marge.\n";
        return;
    }

    std::cout << m_name << ": transferred " << kMargeAllowance << " to Marge.\n";
}

void Homer::PayElectricity() const
{
    if (!m_bank.TrySendMoney(m_accountId, m_burnsAccountId, kElectricityBill))
    {
        std::cout << m_name << ": not enough funds to pay for electricity.\n";
        return;
    }

    std::cout << m_name << ": payed " << kElectricityBill << " for electricity.\n";
}

void Homer::GiveCashToChild(IActor* child, const std::string& childName) const
{
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

AccountId Homer::GetAccountId() const
{
    return m_accountId;
}

void Homer::SetMarge(Marge* marge)
{
    m_marge = marge;
}

void Homer::SetBart(Bart* bart)
{
    m_bart = bart;
}

void Homer::SetLisa(Lisa* lisa)
{
    m_lisa = lisa;
}

void Homer::SetBurnsAccountId(const AccountId burnsAccountId)
{
    m_burnsAccountId = burnsAccountId;
}