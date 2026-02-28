#include "WordFrequency.h"

#include <algorithm>
#include <ostream>
#include <string>

std::string ToLower(const std::string & word)
{
    std::string lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return lower;
}

std::map<std::string, int> CountWordFrequencies(std::istream & input)
{
    std::map<std::string, int> frequencies;
    std::string word;

    while (input >> word)
    {
        ++frequencies[ToLower(word)];
    }

    return frequencies;
}

void PrintWordFrequencies(const std::map<std::string, int> & frequencies, std::ostream & output)
{
    for (const auto& [word, count] : frequencies)
    {
        output << word << " " << count << "\n";
    }
}