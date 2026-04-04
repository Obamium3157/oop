#include "EconomyHelper.h"

#include <iostream>
#include <string>
#include <vector>

#include "Bank.h"
#include "IActor.h"


class Bank;
class IActor;

int readIterationCount(int argc, char* argv[])
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

void printFinalState(const std::vector<IActor*>& actors, const Bank& bank)
{
    std::cout << "\n=== Final State ===\n";

    Money totalCash = 0;
    for (const IActor* actor : actors)
    {
        const Money cash = actor->GetCash();
        totalCash += cash;
        std::cout << actor->GetName() << ": amount of cash = " << cash << "\n";
    }

    const Money bankCash = bank.GetCash();
    std::cout << "\nCash amount according to bank data: " << bankCash << "\n";
    std::cout << "Cash amount owned by actors: " << totalCash << "\n";

    if (bankCash == totalCash)
    {
        std::cout << "[OK]\n";
    }
    else
    {
        std::cout << "[ERROR] Cash discrepancy: " << (bankCash - totalCash) << "\n";
    }
}