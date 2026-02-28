#include "PrimeNumbers.h"

#include <vector>

std::set<int> GeneratePrimeNumbersSet(const int upperBound)
{
    if (upperBound < 2)
    {
        return {};
    }

    std::vector<bool> isComposite(upperBound + 1, false);

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