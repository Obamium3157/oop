#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <limits>

#include "Numbers.h"


std::vector<double> ReadNumbers(std::istream & in)
{
    std::vector<double> numbers;
    std::string token;

    while (in >> token)
    {
        std::istringstream stream(token);
        double number;

        stream >> number;

        if (stream.fail() || !stream.eof())
        {
            throw std::invalid_argument("Invalid token: " + token);
        }

        numbers.push_back(number);
    }

    return numbers;
}

double ComputePositiveMean(const std::vector<double> & numbers)
{
    std::vector<double> positiveNumbers;

    std::copy_if(
        numbers.begin(),
        numbers.end(),
        std::back_inserter(positiveNumbers),
        [](const double value) { return value > std::numeric_limits<double>::epsilon(); });

    if (positiveNumbers.empty())
    {
        return 0.0;
    }

    const double sum = std::accumulate(positiveNumbers.begin(), positiveNumbers.end(), 0.0);

    return sum / static_cast<double>(positiveNumbers.size());
}

void ProcessNumbers(std::vector<double> & numbers)
{
    const double positiveMean = ComputePositiveMean(numbers);

    std::transform(
        numbers.begin(),
        numbers.end(),
        numbers.begin(),
        [positiveMean](const double value) { return value + positiveMean; });
}

void PrintSortedNumbers(std::ostream & out, std::vector<double> numbers)
{
    std::sort(numbers.begin(), numbers.end());

    bool isFirst = true;

    for (const double number : numbers)
    {
        constexpr auto precision = 3;
        if (!isFirst)
        {
            out << " ";
        }

        out << std::fixed << std::setprecision(precision) << number;
        isFirst = false;
    }

    if (!numbers.empty())
    {
        out << "\n";
    }
}