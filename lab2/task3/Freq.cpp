#include <iostream>

#include "WordFrequency.h"

int main()
{
    const auto frequencies = CountWordFrequencies(std::cin);
    PrintWordFrequencies(frequencies, std::cout);

    return 0;
}
