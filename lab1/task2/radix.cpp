// radix - 100 баллов

#include <algorithm>
#include <exception>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

constexpr int MIN_RADIX = 2;
constexpr int MAX_RADIX = 36;

void PrintErrorMessage()
{
    std::cout << "ERROR\n";
}

void ValidateRadixRange(const int radix)
{
    if (radix < MIN_RADIX || radix > MAX_RADIX)
    {
        throw std::invalid_argument("radix out of range");
    }
}

int CharToDigit(const char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }

    if (c >= 'A' && c <= 'Z')
    {
        return 10 + (c - 'A');
    }

    if (c >= 'a' && c <= 'z')
    {
        return 10 + (c - 'a');
    }

    return -1;
}

std::pair<std::size_t, bool> ParseSign(const std::string& str)
{
    if (str.empty())
    {
        throw std::invalid_argument("empty input");
    }

    bool negative = false;
    std::size_t pos = 0;

    if (str[0] == '+' || str[0] == '-')
    {
        negative = str[0] == '-';
        pos = 1;

        if (pos == str.size())
        {
            throw std::invalid_argument("sigh without digits");
        }
    }

    return { pos, negative };
}

int ParseInt(const std::string& str, const int radix)
{
    ValidateRadixRange(radix);

    const auto [startPos, negative] = ParseSign(str);

    constexpr int intMin = std::numeric_limits<int>::min();
    constexpr int intMax = std::numeric_limits<int>::max();

    int result = 0;

    for (std::size_t i = startPos; i < str.size(); ++i)
    {
        const int digit = CharToDigit(str[i]);

        if (digit < 0 || digit >= radix)
        {
            throw std::invalid_argument("invalid digit");
        }

        if (!negative)
        {
            if (result > (intMax - digit) / radix)
            {
                throw std::overflow_error("overflow");
            }

            result = result * radix + digit;
        }
        else
        {
            if (result < (intMin + digit) / radix)
            {
                throw std::overflow_error("overflow");
            }

            result = result * radix - digit;
        }
    }

    return result;
}

char DigitToChar(const unsigned digit)
{
    if (digit < 10)
    {
        return static_cast<char>('0' + digit);
    }

    return static_cast<char>('A' + digit - 10);
}

std::string ToString(const int value, const int radix)
{
    ValidateRadixRange(radix);

    if (value == 0)
    {
        return "0";
    }

    bool negative = value < 0;

    unsigned int magnitude =
        negative
            ? static_cast<unsigned int>(-(static_cast<long long>(value)))
            : static_cast<unsigned int>(value);

    std::string result;

    while (magnitude > 0)
    {
        const unsigned digit = magnitude % radix;
        result.push_back(DigitToChar(digit));
        magnitude /= radix;
    }

    if (negative)
    {
        result.push_back('-');
    }

    std::reverse(result.begin(), result.end());
    return result;
}

int ParseRadix(const std::string& str)
{
    if (str.empty())
    {
        throw std::invalid_argument("empty radix");
    }

    for (const char c : str)
    {
        if (c < '0' || c > '9')
        {
            throw std::invalid_argument("invalid radix");
        }
    }

    const int radix = std::stoi(str);
    ValidateRadixRange(radix);

    return radix;
}

int main(const int argc, char* argv[])
{
    if (argc != 4)
    {
        PrintErrorMessage();
        return 1;
    }

    try
    {
        const int sourceRadix = ParseRadix(argv[1]);
        const int targetRadix = ParseRadix(argv[2]);
        const std::string valueStr = argv[3];

        const int value = ParseInt(valueStr, sourceRadix);
        const std::string converted = ToString(value, targetRadix);

        std::cout << converted << '\n';
        return 0;
    }
    catch (std::exception&)
    {
        PrintErrorMessage();
        return 1;
    }
}