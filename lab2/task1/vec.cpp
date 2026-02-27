#include <iostream>

#include "numbers.h"

int main()
{
    try
    {
        std::vector<double> numbers = ReadNumbers();
        ProcessNumbers(numbers);
        PrintSortedNumbers(numbers);
    }
    catch (const std::invalid_argument&)
    {
        std::cout << "ERROR\n";
    }

    return 0;
}