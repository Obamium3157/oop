#include <iostream>

#include "Numbers.h"

int main()
{
    try
    {
        std::vector<double> numbers = ReadNumbers(std::cin);
        ProcessNumbers(numbers);
        PrintSortedNumbers(std::cout, numbers);
    }
    catch (const std::invalid_argument&)
    {
        std::cout << "ERROR\n";
    }

    return 0;
}