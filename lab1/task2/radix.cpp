// radix - 100 баллов

#include <exception>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

constexpr int MIN_RADIX_VALUE = 2;
constexpr int MAX_RADIX_VALUE = 36;

void PrintErrorMessage()
{
    std::cout << "ERROR\n";
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

void ValidateRadix(const int radix)
{
    if (radix < MIN_RADIX_VALUE || radix > MAX_RADIX_VALUE)
    {
        throw std::invalid_argument("radix out of range");
    }
}

unsigned long long IntToULL(const int n)
{
    if (n < 0)
    {
        return static_cast<unsigned long long>(-static_cast<long long>(n));
    }

    return static_cast<unsigned long long>(n);
}

std::string BuildReversedDigitsWithSign(const int n, unsigned long long & unum, const int radix)
{
    std::string out;
    while (unum > 0)
    {
        auto digit = static_cast<unsigned int>(unum % static_cast<unsigned long long>(radix));
        if (digit < 10)
        {
            out.push_back(static_cast<char>('0' + digit));
        }
        else
        {
            out.push_back(static_cast<char>('A' + (digit - 10)));
        }
        unum /= radix;
    }

    if (n < 0)
    {
        out.push_back('-');
    }

    return out;
}

void ReverseString(std::string & out)
{
    for (size_t i = 0, j = out.size() - 1; i < j; ++i, --j)
    {
        const char tmp = out[i];
        out[i] = out[j];
        out[j] = tmp;
    }
}

std::pair<size_t, bool> ParseSignPrefix(const std::string& str)
{
    if (str.empty())
    {
        throw std::invalid_argument("empty input");
    }

    size_t pos = 0;
    bool negative = false;
    if (str[0] == '+' || str[0] == '-')
    {
        negative = (str[0] == '-');
        pos = 1;
        if (pos == str.size())
        {
            throw std::invalid_argument("only sign, no digits");
        }
    }

    return { pos, negative };
}

int StringToInt(const std::string& str, const int radix)
{
    ValidateRadix(radix);

    auto parseResult = ParseSignPrefix(str);
    auto [pos, negative] = parseResult;

    constexpr auto intMax = static_cast<long long>(std::numeric_limits<int>::max());
    constexpr long long negLimit = intMax + 1LL;

    long long acc = 0;
    for (; pos < str.size(); ++pos)
    {
        const char c = str[pos];
        const int digit = CharToDigit(c);
        if (digit < 0 || digit >= radix)
        {
            throw std::invalid_argument("invalid digit for radix");
        }

        if (!negative)
        {
            if (acc > (intMax - digit) / radix)
            {
                throw std::overflow_error("overflow");
            }
        }
        else
        {
            if (acc > (negLimit - digit) / radix)
            {
                throw std::overflow_error("overflow");
            }
        }

        acc = acc * radix + digit;
    }

    if (!negative)
    {
        return static_cast<int>(acc);
    }

    const long long result = -acc;
    if (result < static_cast<long long>(std::numeric_limits<int>::min()))
    {
        throw std::overflow_error("overflow");
    }
    return static_cast<int>(result);
}

std::string IntToString(const int n, const int radix)
{
    ValidateRadix(radix);

    if (n == 0)
    {
        return "0";
    }

    unsigned long long unum = IntToULL(n);
    std::string out = BuildReversedDigitsWithSign(n, unum, radix);

    ReverseString(out);

    return out;
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
        const std::string srcRadStr = argv[1];
        const std::string dstRadStr = argv[2];

        for (const char c : srcRadStr)
        {
            if (c < '0' || c > '9')
            {
                throw std::invalid_argument("invalid source radix");
            }
        }
        for (const char c : dstRadStr)
        {
            if (c < '0' || c > '9')
            {
                throw std::invalid_argument("invalid destination radix");
            }
        }

        const int srcRad = std::stoi(srcRadStr);
        const int dstRad = std::stoi(dstRadStr);

        if (srcRad < MIN_RADIX_VALUE
            || srcRad > MAX_RADIX_VALUE
            || dstRad < MIN_RADIX_VALUE
            || dstRad > MAX_RADIX_VALUE)
        {
            throw std::invalid_argument("radix out of range");
        }

        const std::string valueStr = argv[3];

        const int value = StringToInt(valueStr, srcRad);
        const std::string out = IntToString(value, dstRad);
        std::cout << out << '\n';
        return 0;
    }
    catch (const std::exception&)
    {
        PrintErrorMessage();
        return 1;
    }
}