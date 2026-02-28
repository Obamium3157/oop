#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <vector>
#include <sstream>

#include "Numbers.h"

using Catch::Matchers::WithinAbs;

static constexpr double epsilon = 1e-6;

TEST_CASE("ProcessNumbers: example case 1 (positive values only)")
{
    std::vector<double> numbers = {1.0, 2.0, 3.659512};
    ProcessNumbers(numbers);

    REQUIRE_THAT(numbers[0], WithinAbs(3.2198373, epsilon));
    REQUIRE_THAT(numbers[1], WithinAbs(4.2198373, epsilon));
    REQUIRE_THAT(numbers[2], WithinAbs(5.8793493, epsilon));
}

TEST_CASE("ProcessNumbers: example case 2 (mixed values)")
{
    std::vector<double> numbers = {4.0, 16.0, -30.0, 10.0};
    ProcessNumbers(numbers);

    REQUIRE_THAT(numbers[0], WithinAbs(14.0, epsilon));
    REQUIRE_THAT(numbers[1], WithinAbs(26.0, epsilon));
    REQUIRE_THAT(numbers[2], WithinAbs(-20.0, epsilon));
    REQUIRE_THAT(numbers[3], WithinAbs(20.0, epsilon));
}

TEST_CASE("ProcessNumbers: example case 3 (negative values only)")
{
    std::vector<double> numbers = {-1.0004, -703.0, -3.659512, -11.0};
    const std::vector<double> original = numbers;
    ProcessNumbers(numbers);

    for (std::size_t i = 0; i < numbers.size(); ++i)
    {
        REQUIRE_THAT(numbers[i], WithinAbs(original[i], epsilon));
    }
}

TEST_CASE("ProcessNumbers: empty vector")
{
    std::vector<double> numbers = {};
    ProcessNumbers(numbers);
    REQUIRE(numbers.empty());
}

TEST_CASE("ProcessNumbers: single positive (it should double)")
{
    std::vector<double> numbers = {5.0};
    ProcessNumbers(numbers);
    REQUIRE_THAT(numbers[0], WithinAbs(10.0, epsilon));
}

TEST_CASE("PrintSortedNumbers: does not modify original vector")
{
    std::vector<double> numbers = {3.0, 1.0, 2.0};
    const std::vector<double> original = numbers;
    PrintSortedNumbers(std::cout, numbers);
    REQUIRE(numbers == original);
}

TEST_CASE("PrintSortedNumbers: empty vector produces no output")
{
    std::ostringstream output;
    std::streambuf* oldCoutBuf = std::cout.rdbuf(output.rdbuf());

    PrintSortedNumbers(std::cout, {});

    std::cout.rdbuf(oldCoutBuf);
    REQUIRE(output.str().empty());
}

TEST_CASE("PrintSortedNumbers: output is sorted and formatted")
{
    std::ostringstream output;
    std::streambuf* oldCoutBuf = std::cout.rdbuf(output.rdbuf());

    PrintSortedNumbers(std::cout, {3.0, 1.0, 2.5});

    std::cout.rdbuf(oldCoutBuf);
    REQUIRE(output.str() == "1.000 2.500 3.000\n");
}

TEST_CASE("ReadNumbers: valid input")
{
    std::istringstream input("1.0 2.5 -3.0");
    std::streambuf* oldCinBuf = std::cin.rdbuf(input.rdbuf());

    const std::vector<double> numbers = ReadNumbers(std::cin);

    std::cin.rdbuf(oldCinBuf);
    REQUIRE(numbers.size() == 3);
    REQUIRE_THAT(numbers[0], WithinAbs(1.0, epsilon));
    REQUIRE_THAT(numbers[1], WithinAbs(2.5, epsilon));
    REQUIRE_THAT(numbers[2], WithinAbs(-3.0, epsilon));
}

TEST_CASE("ReadNumbers: empty input")
{
    std::istringstream input("");
    std::streambuf* oldCinBuf = std::cin.rdbuf(input.rdbuf());

    const std::vector<double> numbers = ReadNumbers(std::cin);

    std::cin.rdbuf(oldCinBuf);
    REQUIRE(numbers.empty());
}

TEST_CASE("ReadNumbers: invalid token throws")
{
    std::istringstream input("1.0 abc 3.0");
    std::streambuf* oldCinBuf = std::cin.rdbuf(input.rdbuf());

    REQUIRE_THROWS_AS(ReadNumbers(std::cin), std::invalid_argument);

    std::cin.rdbuf(oldCinBuf);
}

TEST_CASE("ReadNumbers: lone minus sign throws")
{
    std::istringstream input("1.0 - 3.0");
    std::streambuf* oldCinBuf = std::cin.rdbuf(input.rdbuf());

    REQUIRE_THROWS_AS(ReadNumbers(std::cin), std::invalid_argument);

    std::cin.rdbuf(oldCinBuf);
}

TEST_CASE("ProcessNumbers: single negative element unchanged")
{
    std::vector<double> numbers = {-7.0};
    ProcessNumbers(numbers);
    REQUIRE_THAT(numbers[0], WithinAbs(-7.0, epsilon));
}

TEST_CASE("ProcessNumbers: single zero element unchanged")
{
    std::vector<double> numbers = {0.0};
    ProcessNumbers(numbers);
    REQUIRE_THAT(numbers[0], WithinAbs(0.0, epsilon));
}

TEST_CASE("ProcessNumbers: all elements equal")
{
    std::vector<double> numbers = {3.0, 3.0, 3.0};
    ProcessNumbers(numbers);
    REQUIRE_THAT(numbers[0], WithinAbs(6.0, epsilon));
    REQUIRE_THAT(numbers[1], WithinAbs(6.0, epsilon));
    REQUIRE_THAT(numbers[2], WithinAbs(6.0, epsilon));
}