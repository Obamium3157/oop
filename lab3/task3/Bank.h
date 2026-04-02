#ifndef OOP_BANK_H
#define OOP_BANK_H

#include <map>
#include <vector>

#include "BankOperationError.h"

using AccountId = unsigned long long;
using Money = long long;
using AccountTable = std::map<AccountId, Money>;

class Bank
{
public:
    /**
     * @param cash количество денег в наличном обороте
     * @throws BankOperationError отрицательное количество денег
     */
    explicit Bank(Money cash);

    Bank(const Bank&) = delete;
    Bank& operator=(const Bank&) = delete;

    /**
     * @param srcAccountId исходный счет
     * @param dstAccountId целевой счет
     * @param amount сумма перевода
     * @throws BankOperationError отсутствие счета
     * @throws BankOperationError недостаточно денег на исходном счете
     * @throws std::out_of_range отрицательное количество переводимых денег
     */
    void SendMoney(AccountId srcAccountId, AccountId dstAccountId, Money amount);

    /**
     * @param srcAccountId исходный счет
     * @param dstAccountId целевой счет
     * @param amount сумма перевода
     * @returns true — перевод успешный
     * @returns false — на исходном счете недостаточно средств
     * @throws BankOperationError номера счетов невалидны
     * @throws std::out_of_range отрицательное количество денег
     */
    [[nodiscard]] bool TrySendMoney(AccountId srcAccountId, AccountId dstAccountId, Money amount);

    [[nodiscard]] Money GetCash() const;

    /**
     * @throws BankOperationError указанный счет отсутствует
     */
    Money GetAccountBalance(AccountId accountId);

    /**
     * Снимает деньги со счёта.
     * Снятые деньги переходят добавляются к массе наличных денег
     *
     * @throws BankOperationError невалидный номер счета
     * @throws BankOperationError отсутствие достаточного количества денег
     * @throws std::out_of_range отрицательное количество денег
     */
    void WithdrawMoney(AccountId account, Money amount);

    /**
     * @returns false — нехватка денег на счете; в этом случае количество наличных денег остается неизменным
     * @throws BankOperationError невалидный номер аккаунта
     * @throws std::out_of_range отрицательное количество денег
     */
    [[nodiscard]] bool TryWithdrawMoney(AccountId account, Money amount);

    /**
     * Поместить наличные деньги на счёт. Количество денег в наличном обороте
     * уменьшается на величину amount.
     *
     * Нельзя поместить больше, чем имеется денег в наличном обороте
     * @throws BankOperationError невалидный номер аккаунта
     * @throws BankOperationError нехватка наличных денег в обороте
     * @throws std::out_of_range отрицательное количество денег
     */
    void DepositMoney(AccountId account, Money amount);

    /**
     * Открывает счёт в банке. После открытия счёта на нём нулевой баланс.
     *
     * Каждый открытый счёт имеет уникальный номер.
     *
     * @return номер счета
     */
    [[nodiscard]] AccountId OpenAccount();

    /**
     * Закрывает указанный счёт.
     *
     * @return количество денег, которые были на счёте в момент закрытия.
     * Эти деньги переходят в наличный оборот
     */
    [[nodiscard]] Money CloseAccount(AccountId accountId);

private:
    /**
     * @throws BankOperationError
     */
    static void ThrowAccountDoesNotExist(AccountId accountId);
    /**
     * @throws BankOperationError
     */
    static void ThrowNotEnoughMoney(AccountId accountId, Money amount);
    /**
     * @throws std::out_of_range
     */
    static void ValidateMoneyAmount(Money money);

    bool AccountExists(AccountId accountId) const;
    /**
     * @throws BankOperationError если аккаунт не существует
     */
    void ValidateAccounts(const std::vector<AccountId>& accountIds) const;
    /**
     * @throws BankOperationError если на аккауете меньше денег, чем сумма перевода
     */
    void ValidateTransaction(AccountId srcAccountId, Money amount);
    /**
     * @throws BankOperationError если сумма не входит в наличный оборот
     */
    void CheckMoneyAmountFitsCashTurnover(Money money);

    Money m_cash;
    AccountTable m_accounts;

    static inline AccountId s_currentMaxAccountId = 0;
};


#endif //OOP_BANK_H
