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

    try
    {
        printFinalState(actors, bank);
    } catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
