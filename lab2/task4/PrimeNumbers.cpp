#include "PrimeNumbers.h"

#include <vector>

std::set<int> GeneratePrimeNumbersSet(const int upperBound)
{
    constexpr int maxUpperBound = 100'000'000;
    if (upperBound < 2 || upperBound > maxUpperBound)
    {
        return {};
    }


    std::vector<bool> isComposite(upperBound + 1, false);

    // TODO: ускорить в четыре раза
    for (int i = 2; static_cast<long long>(i) * i <= upperBound; ++i)
    {
        if (isComposite[i])
        {
            continue;
        }

        for (int j = i * i; j <= upperBound; j += i)
        {
            isComposite[j] = true;
        }
    }

    std::set<int> primes;

    for (int i = 2; i <= upperBound; ++i)
    {
        if (!isComposite[i])
        {
            primes.insert(i);
        }
    }

    return primes;
}