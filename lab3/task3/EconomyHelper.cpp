#include "EconomyHelper.h"

#include <iostream>
#include <string>
#include <vector>

#include "Bank.h"
#include "IActor.h"

namespace
{
    Money CalculateTotalCash(const std::vector<IActor*>& actors)
    {
        Money totalCash = 0;
        for (const IActor* actor : actors)
        {
            const Money cash = actor->GetCash();
            totalCash += cash;
        }

        return totalCash;
    }

    void PrintActorCash(const IActor* actor)
    {
        std::cout << actor->GetName() << ": amount of cash = " << actor->GetCash() << "\n";
    }

    void PrintActorsCash(const std::vector<IActor*>& actors)
    {
        for (const IActor* actor : actors)
        {
            PrintActorCash(actor);
        }
    }
}

int ReadIterationCount(int argc, char* argv[])
{
    if (argc >= 2)
    {
        return std::stoi(argv[1]);
    }

    int count = 0;
    std::cout << "Enter amount of iterations: ";
    std::cin >> count;
    return count;
}

void AssertFinalState(const std::vector<IActor*>& actors, const Bank& bank)
{
    const Money totalCash = CalculateTotalCash(actors);
    PrintActorsCash(actors);

    const Money bankCash = bank.GetCash();
    std::cout << "\nCash amount according to bank data: " << bankCash << "\n";
    std::cout << "Cash amount owned by actors: " << totalCash << "\n";

    if (bankCash != totalCash)
    {
        throw BankOperationError(std::format("[ERROR] Cash discrepancy: {}", bankCash - totalCash));
    }
}

void PrintFinalState(const std::vector<IActor*>& actors, const Bank& bank)
{
    std::cout << "\n=== Final State ===\n";

    try
    {
        AssertFinalState(actors, bank);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
        return;
    }

    std::cout << "[OK]\n";
}
