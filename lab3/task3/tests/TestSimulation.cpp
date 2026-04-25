#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <string>

#include "../Bank.h"
#include "../SimulationContext.h"
#include "../EconomyHelper.h"

TEST_CASE("Тесты симуляции", "[Simulation][Run]")
{
    const int iterations = GENERATE(0, 1, 10, 50, 100, 500, 1000);

    SECTION("Симуляция выполняется без ошибок для " + std::to_string(iterations) + " шагов")
    {
        constexpr Money kInitialCash = 1000;
        Bank bank(kInitialCash);
        const SimulationContext context(bank);
        const auto& actors = context.GetActors();

        auto runSimulation = [&]()
        {
            for (int step = 0; step < iterations; ++step)
            {
                for (IActor* actor : actors)
                {
                    actor->Act();
                }
            }
        };

        REQUIRE_NOTHROW(runSimulation());

        REQUIRE(bank.GetCash() >= 0);

        REQUIRE_NOTHROW(printFinalState(actors, bank));
    }
}