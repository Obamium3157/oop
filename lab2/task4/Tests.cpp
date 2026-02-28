#include <catch2/catch_test_macros.hpp>
#include <set>

#include "PrimeNumbers.h"

TEST_CASE("GeneratePrimeNumbersSet: upperBound = 0 returns empty set")
{
    REQUIRE(GeneratePrimeNumbersSet(0).empty());
}

TEST_CASE("GeneratePrimeNumbersSet: upperBound = 1 returns empty set")
{
    REQUIRE(GeneratePrimeNumbersSet(1).empty());
}

TEST_CASE("GeneratePrimeNumbersSet: upperBound = 2 returns {2}")
{
    REQUIRE(GeneratePrimeNumbersSet(2) == std::set<int>{2});
}

TEST_CASE("GeneratePrimeNumbersSet: upperBound = 4 returns {2, 3}")
{
    REQUIRE(GeneratePrimeNumbersSet(4) == std::set<int>{2, 3});
}

TEST_CASE("GeneratePrimeNumbersSet: upperBound = 3 returns {2, 3}")
{
    REQUIRE(GeneratePrimeNumbersSet(3) == std::set<int>{2, 3});
}

TEST_CASE("GeneratePrimeNumbersSet: negative upperBound returns empty set")
{
    REQUIRE(GeneratePrimeNumbersSet(-5).empty());
}

TEST_CASE("GeneratePrimeNumbersSet: upperBound = 10 returns {2, 3, 5, 7}")
{
    REQUIRE(GeneratePrimeNumbersSet(10) == std::set<int>{2, 3, 5, 7});
}

TEST_CASE("GeneratePrimeNumbersSet: upperBound = 20 returns correct set")
{
    REQUIRE(GeneratePrimeNumbersSet(20) == std::set<int>{2, 3, 5, 7, 11, 13, 17, 19});
}

TEST_CASE("GeneratePrimeNumbersSet: upperBound = 30 returns correct set")
{
    REQUIRE(GeneratePrimeNumbersSet(30) == std::set<int>{2, 3, 5, 7, 11, 13, 17, 19, 23, 29});
}

TEST_CASE("GeneratePrimeNumbersSet: upperBound itself is prime and included")
{
    const auto primes = GeneratePrimeNumbersSet(13);
    REQUIRE(primes.count(13) == 1);
}

TEST_CASE("GeneratePrimeNumbersSet: upperBound itself is composite and not included")
{
    const auto primes = GeneratePrimeNumbersSet(15);
    REQUIRE(primes.count(15) == 0);
    REQUIRE(primes.count(13) == 1);
}

TEST_CASE("GeneratePrimeNumbersSet: no primes exceed upperBound")
{
    const auto primes = GeneratePrimeNumbersSet(50);
    for (const int prime : primes)
    {
        REQUIRE(prime <= 50);
    }
}

TEST_CASE("GeneratePrimeNumbersSet: all returned numbers are prime")
{
    const auto primes = GeneratePrimeNumbersSet(100);

    for (const int candidate : primes)
    {
        bool isPrime = candidate >= 2;
        for (int i = 2; i * i <= candidate; ++i)
        {
            if (candidate % i == 0)
            {
                isPrime = false;
                break;
            }
        }
        REQUIRE(isPrime);
    }
}

TEST_CASE("GeneratePrimeNumbersSet: count up to 100 is 25")
{
    REQUIRE(GeneratePrimeNumbersSet(100).size() == 25);
}

TEST_CASE("GeneratePrimeNumbersSet: count up to 1000 is 168")
{
    REQUIRE(GeneratePrimeNumbersSet(1000).size() == 168);
}

TEST_CASE("GeneratePrimeNumbersSet: count up to 100000000 is 5761455")
{
    REQUIRE(GeneratePrimeNumbersSet(100'000'000).size() == 5'761'455);
}