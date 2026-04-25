#include "Bankable.h"

Bankable::Bankable(Bank& bank)
    : m_bank(bank)
    , m_accountId(bank.OpenAccount())
{
}

std::optional<AccountId> Bankable::GetBankAccountId() const
{
    return m_accountId;
}