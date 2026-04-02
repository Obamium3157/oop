#include "Bank.h"

#include <format>

#include "BankOperationError.h"

Bank::Bank(const Money cash)
    : m_accounts{}
{
    if (cash < 0)
    {
        throw BankOperationError("Cash amount is negative");
    }
    m_cash = cash;
}

void Bank::SendMoney(const AccountId srcAccountId, const AccountId dstAccountId, const Money amount)
{
    ValidateAccounts({srcAccountId, dstAccountId});
    ValidateMoneyAmount(amount);
    ValidateTransaction(srcAccountId, amount);

    m_accounts[srcAccountId] -= amount;
    m_accounts[dstAccountId] += amount;
}

bool Bank::TrySendMoney(const AccountId srcAccountId, const AccountId dstAccountId, const Money amount)
{
    try
    {
        SendMoney(srcAccountId, dstAccountId, amount);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

Money Bank::GetCash() const
{
    return m_cash;
}

Money Bank::GetAccountBalance(AccountId accountId)
{
    ValidateAccounts({accountId});

    return m_accounts[accountId];
}

void Bank::WithdrawMoney(AccountId account, const Money amount)
{
    ValidateAccounts({account});
    ValidateMoneyAmount(amount);
    ValidateTransaction(account, amount);

    m_accounts[account] -= amount;
    m_cash += amount;
}

bool Bank::TryWithdrawMoney(const AccountId account, const Money amount)
{
    try
    {
        WithdrawMoney(account, amount);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

void Bank::DepositMoney(AccountId account, const Money amount)
{
    ValidateAccounts({account});
    ValidateMoneyAmount(amount);
    CheckMoneyAmountFitsCashTurnover(amount);

    m_accounts[account] += amount;
    m_cash -= amount;
}

AccountId Bank::OpenAccount()
{
    const AccountId accountId = s_currentMaxAccountId++;
    m_accounts[accountId] = 0;

    return accountId;
}

Money Bank::CloseAccount(const AccountId accountId)
{
    if (!m_accounts.contains(accountId))
    {
        return 0;
    }

    const Money money = m_accounts[accountId];
    m_cash += money;
    m_accounts.erase(accountId);

    return money;
}

void Bank::ThrowAccountDoesNotExist(AccountId accountId)
{
    throw BankOperationError(std::format("Account {} does not exist", accountId));
}

void Bank::ThrowNotEnoughMoney(AccountId accountId, Money amount)
{
    throw BankOperationError(std::format(
        "Account {} does not have enough money ({})", accountId, amount
    ));
}

void Bank::ValidateMoneyAmount(const Money money)
{
    if (money < 0)
    {
        throw std::out_of_range("Money amount cannot be negative");
    }
}


bool Bank::AccountExists(const AccountId accountId) const
{
    return m_accounts.contains(accountId);
}

void Bank::ValidateAccounts(const std::vector<AccountId>& accountIds) const
{
    for (const auto accountId : accountIds)
    {
        if (!AccountExists(accountId))
        {
            ThrowAccountDoesNotExist(accountId);
        }
    }
}

void Bank::ValidateTransaction(const AccountId srcAccountId, const Money amount)
{
    if (m_accounts[srcAccountId] < amount)
    {
        ThrowNotEnoughMoney(srcAccountId, amount);
    }
}

void Bank::CheckMoneyAmountFitsCashTurnover(Money money)
{
    if (m_cash < money)
    {
        throw BankOperationError(std::format(
            "{} amount of money does not fit cash turnover ({})", money, m_cash
        ));
    }
}
