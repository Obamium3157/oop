#include <iostream>

#include "Bank.h"

struct Actor
{
    AccountId id;
    Money balance;

    void PrintData() const
    {
        std::cout << "ID: " << id << "\tBalance: " << balance << std::endl;
    }
};


int main()
{
    Bank bank(1000);
    auto actor1 = Actor{bank.OpenAccount(), 100};
    auto actor2 = Actor{bank.OpenAccount(), 900};

    actor1.PrintData();
    actor2.PrintData();

    bank.DepositMoney(actor1.id, actor1.balance / 2);
    bank.DepositMoney(actor2.id, actor2.balance);

    std::cout << bank.GetAccountBalance(actor1.id) << std::endl;
    std::cout << bank.GetAccountBalance(actor2.id) << std::endl;

    return 0;
}