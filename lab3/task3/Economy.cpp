#include "Bank.h"
#include "Homer.h"
#include "Marge.h"
#include "Bart.h"
#include "Lisa.h"
#include "Apu.h"
#include "Burns.h"

#include <iostream>
#include <vector>

namespace
{
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
            std::cout << "[ERROR] Cash disperancy: " << (bankCash - totalCash) << "\n";
        }
    }
}

int main(int argc, char* argv[])
{
    const int iterations = readIterationCount(argc, argv);

    constexpr Money kInitialCash = 1000;
    Bank bank(kInitialCash);

    Burns burns(bank, 0);
    Homer homer(bank, 0);
    Marge marge(bank, 0);
    Bart bart(0);
    Lisa lisa(0);
    Apu apu(bank, 0);

    bank.DepositMoney(burns.GetAccountId(), kInitialCash);

    burns.SetHomerAccountId(homer.GetAccountId());

    homer.SetBurnsAccountId(burns.GetAccountId());
    homer.SetMarge(&marge);
    homer.SetBart(&bart);
    homer.SetLisa(&lisa);

    marge.SetApu(&apu);
    bart.SetApu(&apu);
    lisa.SetApu(&apu);

    apu.SetBurnsAccountId(burns.GetAccountId());

    const std::vector<IActor*> actors = {&burns, &homer, &marge, &bart, &lisa, &apu};

    for (int step = 0; step < iterations; ++step)
    {
        std::cout << "\nStep " << (step + 1) << ":\n";
        for (IActor* actor : actors)
        {
            actor->Act();
        }
    }

    printFinalState(actors, bank);
    return 0;
}
