#include "Bank.h"
#include "EconomyHelper.h"
#include "SimulationContext.h"

#include <iostream>

int main(int argc, char* argv[])
{
    const int iterations = readIterationCount(argc, argv);

    constexpr Money kInitialCash = 1000;
    Bank bank(kInitialCash);
    const SimulationContext context(bank);
    const auto& actors = context.GetActors();

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
