#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include "../Bank.h"
#include "../BankOperationError.h"

namespace
{
    constexpr Money INITIAL_CASH = 10000;
    constexpr Money ZERO = 0;
    constexpr Money NEGATIVE_AMOUNT = -1;
}

TEST_CASE("Bank: конструктор", "[Bank][Constructor]")
{
    SECTION("Положительное количество наличных")
    {
        REQUIRE_NOTHROW(Bank{INITIAL_CASH});
        Bank bank{INITIAL_CASH};
        REQUIRE(bank.GetCash() == INITIAL_CASH);
    }

    SECTION("Нулевое количество наличных")
    {
        REQUIRE_NOTHROW(Bank{ZERO});
        Bank bank{ZERO};
        REQUIRE(bank.GetCash() == ZERO);
    }

    SECTION("Отрицательное количество наличных бросает BankOperationError")
    {
        REQUIRE_THROWS_AS(Bank{NEGATIVE_AMOUNT}, BankOperationError);
    }
}

TEST_CASE("Bank: открытие счёта", "[Bank][OpenAccount]")
{
    Bank bank{INITIAL_CASH};

    SECTION("Новый счёт имеет нулевой баланс")
    {
        const AccountId id = bank.OpenAccount();
        REQUIRE(bank.GetAccountBalance(id) == ZERO);
    }

    SECTION("Каждый счёт имеет уникальный номер")
    {
        const AccountId first = bank.OpenAccount();
        const AccountId second = bank.OpenAccount();
        REQUIRE(first != second);
    }

    SECTION("Количество наличных не меняется при открытии счёта")
    {
        bank.OpenAccount();
        REQUIRE(bank.GetCash() == INITIAL_CASH);
    }
}

TEST_CASE("Bank: закрытие счёта", "[Bank][CloseAccount]")
{
    Bank bank{INITIAL_CASH};

    SECTION("Деньги со счёта переходят в наличный оборот")
    {
        const AccountId id = bank.OpenAccount();
        bank.DepositMoney(id, 500);
        const Money returned = bank.CloseAccount(id);
        REQUIRE(returned == 500);
        REQUIRE(bank.GetCash() == INITIAL_CASH);
    }

    SECTION("Закрытие пустого счёта возвращает ноль")
    {
        const AccountId id = bank.OpenAccount();
        const Money returned = bank.CloseAccount(id);
        REQUIRE(returned == ZERO);
        REQUIRE(bank.GetCash() == INITIAL_CASH);
    }

    SECTION("После закрытия счёт недоступен")
    {
        const AccountId id = bank.OpenAccount();
        bank.CloseAccount(id);
        REQUIRE_THROWS_AS(bank.GetAccountBalance(id), BankOperationError);
    }

    SECTION("Закрытие несуществующего счёта возвращает ноль")
    {
        constexpr AccountId nonExistentId = 999999;
        REQUIRE(bank.CloseAccount(nonExistentId) == ZERO);
    }
}

TEST_CASE("Bank: получение баланса", "[Bank][GetAccountBalance]")
{
    Bank bank{INITIAL_CASH};

    SECTION("Несуществующий счёт бросает BankOperationError")
    {
        constexpr AccountId nonExistentId = 999999;
        REQUIRE_THROWS_AS(bank.GetAccountBalance(nonExistentId), BankOperationError);
    }

    SECTION("Баланс существующего счёта корректен")
    {
        const AccountId id = bank.OpenAccount();
        bank.DepositMoney(id, 300);
        REQUIRE(bank.GetAccountBalance(id) == 300);
    }
}

TEST_CASE("Bank: внесение наличных на счёт", "[Bank][DepositMoney]")
{
    Bank bank{INITIAL_CASH};
    const AccountId id = bank.OpenAccount();

    SECTION("Корректное внесение уменьшает наличные и увеличивает баланс счёта")
    {
        bank.DepositMoney(id, 1000);
        REQUIRE(bank.GetAccountBalance(id) == 1000);
        REQUIRE(bank.GetCash() == INITIAL_CASH - 1000);
    }

    SECTION("Внесение нуля не меняет состояние")
    {
        bank.DepositMoney(id, ZERO);
        REQUIRE(bank.GetAccountBalance(id) == ZERO);
        REQUIRE(bank.GetCash() == INITIAL_CASH);
    }

    SECTION("Внесение всех наличных оставляет ноль наличных")
    {
        bank.DepositMoney(id, INITIAL_CASH);
        REQUIRE(bank.GetCash() == ZERO);
        REQUIRE(bank.GetAccountBalance(id) == INITIAL_CASH);
    }

    SECTION("Внесение больше наличных бросает BankOperationError")
    {
        REQUIRE_THROWS_AS(bank.DepositMoney(id, INITIAL_CASH + 1), BankOperationError);
        REQUIRE(bank.GetCash() == INITIAL_CASH);
        REQUIRE(bank.GetAccountBalance(id) == ZERO);
    }

    SECTION("Отрицательная сумма бросает std::out_of_range")
    {
        REQUIRE_THROWS_AS(bank.DepositMoney(id, NEGATIVE_AMOUNT), std::out_of_range);
    }

    SECTION("Несуществующий счёт бросает BankOperationError")
    {
        constexpr AccountId nonExistentId = 999999;
        REQUIRE_THROWS_AS(bank.DepositMoney(nonExistentId, 100), BankOperationError);
        REQUIRE(bank.GetCash() == INITIAL_CASH);
    }
}

TEST_CASE("Bank: снятие денег со счёта", "[Bank][WithdrawMoney]")
{
    Bank bank{INITIAL_CASH};
    const AccountId id = bank.OpenAccount();
    bank.DepositMoney(id, 5000);

    SECTION("Корректное снятие уменьшает баланс и увеличивает наличные")
    {
        bank.WithdrawMoney(id, 1000);
        REQUIRE(bank.GetAccountBalance(id) == 4000);
        REQUIRE(bank.GetCash() == INITIAL_CASH - 5000 + 1000);
    }

    SECTION("Снятие нуля не меняет состояние")
    {
        bank.WithdrawMoney(id, ZERO);
        REQUIRE(bank.GetAccountBalance(id) == 5000);
    }

    SECTION("Снятие всего баланса оставляет ноль на счёте")
    {
        bank.WithdrawMoney(id, 5000);
        REQUIRE(bank.GetAccountBalance(id) == ZERO);
    }

    SECTION("Снятие больше баланса бросает BankOperationError")
    {
        REQUIRE_THROWS_AS(bank.WithdrawMoney(id, 5001), BankOperationError);
        REQUIRE(bank.GetAccountBalance(id) == 5000);
    }

    SECTION("Отрицательная сумма бросает std::out_of_range")
    {
        REQUIRE_THROWS_AS(bank.WithdrawMoney(id, NEGATIVE_AMOUNT), std::out_of_range);
    }

    SECTION("Несуществующий счёт бросает BankOperationError")
    {
        constexpr AccountId nonExistentId = 999999;
        REQUIRE_THROWS_AS(bank.WithdrawMoney(nonExistentId, 100), BankOperationError);
    }
}

TEST_CASE("Bank: TryWithdrawMoney", "[Bank][TryWithdrawMoney]")
{
    Bank bank{INITIAL_CASH};
    const AccountId id = bank.OpenAccount();
    bank.DepositMoney(id, 5000);

    SECTION("Возвращает true при успешном снятии")
    {
        REQUIRE(bank.TryWithdrawMoney(id, 1000));
        REQUIRE(bank.GetAccountBalance(id) == 4000);
    }

    SECTION("Возвращает false при нехватке средств, состояние не меняется")
    {
        REQUIRE_FALSE(bank.TryWithdrawMoney(id, 5001));
        REQUIRE(bank.GetAccountBalance(id) == 5000);
    }
}

TEST_CASE("Bank: перевод между счетами", "[Bank][SendMoney]")
{
    Bank bank{INITIAL_CASH};
    const AccountId src = bank.OpenAccount();
    const AccountId dst = bank.OpenAccount();
    bank.DepositMoney(src, 3000);

    SECTION("Корректный перевод изменяет балансы обоих счётов")
    {
        bank.SendMoney(src, dst, 1000);
        REQUIRE(bank.GetAccountBalance(src) == 2000);
        REQUIRE(bank.GetAccountBalance(dst) == 1000);
        REQUIRE(bank.GetCash() == INITIAL_CASH - 3000);
    }

    SECTION("Перевод нуля не меняет балансы")
    {
        bank.SendMoney(src, dst, ZERO);
        REQUIRE(bank.GetAccountBalance(src) == 3000);
        REQUIRE(bank.GetAccountBalance(dst) == ZERO);
    }

    SECTION("Перевод всего баланса")
    {
        bank.SendMoney(src, dst, 3000);
        REQUIRE(bank.GetAccountBalance(src) == ZERO);
        REQUIRE(bank.GetAccountBalance(dst) == 3000);
    }

    SECTION("Недостаточно средств бросает BankOperationError, балансы не меняются")
    {
        REQUIRE_THROWS_AS(bank.SendMoney(src, dst, 3001), BankOperationError);
        REQUIRE(bank.GetAccountBalance(src) == 3000);
        REQUIRE(bank.GetAccountBalance(dst) == ZERO);
    }

    SECTION("Несуществующий счёт-источник бросает BankOperationError")
    {
        constexpr AccountId nonExistentId = 999999;
        REQUIRE_THROWS_AS(bank.SendMoney(nonExistentId, dst, 100), BankOperationError);
    }

    SECTION("Несуществующий счёт-получатель бросает BankOperationError")
    {
        constexpr AccountId nonExistentId = 999999;
        REQUIRE_THROWS_AS(bank.SendMoney(src, nonExistentId, 100), BankOperationError);
    }

    SECTION("Отрицательная сумма бросает std::out_of_range")
    {
        REQUIRE_THROWS_AS(bank.SendMoney(src, dst, NEGATIVE_AMOUNT), std::out_of_range);
    }

    SECTION("Наличные не меняются при переводе между счетами")
    {
        const Money cashBefore = bank.GetCash();
        bank.SendMoney(src, dst, 1000);
        REQUIRE(bank.GetCash() == cashBefore);
    }
}

TEST_CASE("Bank: TrySendMoney", "[Bank][TrySendMoney]")
{
    Bank bank{INITIAL_CASH};
    const AccountId src = bank.OpenAccount();
    const AccountId dst = bank.OpenAccount();
    bank.DepositMoney(src, 3000);

    SECTION("Возвращает true при успешном переводе")
    {
        REQUIRE(bank.TrySendMoney(src, dst, 1000));
        REQUIRE(bank.GetAccountBalance(src) == 2000);
        REQUIRE(bank.GetAccountBalance(dst) == 1000);
    }

    SECTION("Возвращает false при нехватке средств, балансы не меняются")
    {
        REQUIRE_FALSE(bank.TrySendMoney(src, dst, 3001));
        REQUIRE(bank.GetAccountBalance(src) == 3000);
        REQUIRE(bank.GetAccountBalance(dst) == ZERO);
    }
}

TEST_CASE("Bank: инвариант сохранения общей суммы денег", "[Bank][Invariant]")
{
    Bank bank{INITIAL_CASH};
    const AccountId first = bank.OpenAccount();
    const AccountId second = bank.OpenAccount();

    auto totalMoney = [&]() -> Money
    {
        return bank.GetCash()
             + bank.GetAccountBalance(first)
             + bank.GetAccountBalance(second);
    };

    SECTION("Общая сумма не меняется после серии операций")
    {
        REQUIRE(totalMoney() == INITIAL_CASH);

        bank.DepositMoney(first, 2000);
        REQUIRE(totalMoney() == INITIAL_CASH);

        bank.DepositMoney(second, 3000);
        REQUIRE(totalMoney() == INITIAL_CASH);

        bank.SendMoney(first, second, 500);
        REQUIRE(totalMoney() == INITIAL_CASH);

        bank.WithdrawMoney(second, 1000);
        REQUIRE(totalMoney() == INITIAL_CASH);
    }

    SECTION("Общая сумма не меняется после закрытия счёта")
    {
        bank.DepositMoney(first, 4000);
        bank.CloseAccount(first);
        REQUIRE(bank.GetCash() + bank.GetAccountBalance(second) == INITIAL_CASH);
    }
}