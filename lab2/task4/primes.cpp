#include <iostream>
#include <string>

#include "prime_numbers.h"

int main(const int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <upper_bound>\n";
        return 1;
    }

    int upperBound;

    try
    {
        upperBound = std::stoi(argv[1]);
    }
    catch (const std::exception&)
    {
        std::cout << "Error: upper bound must be an integer\n";
        return 1;
    }

    if (upperBound < 0 || upperBound > maxUpperBound)
    {
        std::cout << "Error: upper bound must be between 0 and " << maxUpperBound << "\n";
        return 1;
    }

    for (const auto primes = GeneratePrimeNumbersSet(upperBound); const int prime : primes)
    {
        std::cout << prime << "\n";
    }

    return 0;
}