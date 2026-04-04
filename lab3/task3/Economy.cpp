#include "Bank.h"
#include "Homer.h"
#include "Marge.h"
#include "Bart.h"
#include "Lisa.h"
#include "Apu.h"
#include "Burns.h"
#include "EconomyHelper.h"

#include <iostream>
#include <vector>

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
