#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <vector>

#include "numbers.h"

using Catch::Matchers::WithinAbs;

static constexpr double kEpsilon = 1e-6;

TEST_CASE("ComputePositiveMean: positive values only")
{
    const std::vector<double> numbers = {1.0, 2.0, 3.0};
    REQUIRE_THAT(ComputePositiveMean(numbers), WithinAbs(2.0, kEpsilon));
}

TEST_CASE("ComputePositiveMean: mixed values")
{
    const std::vector<double> numbers = {4.0, 16.0, -30.0, 10.0};
    REQUIRE_THAT(ComputePositiveMean(numbers), WithinAbs(10.0, kEpsilon));
}

TEST_CASE("ComputePositiveMean: negative values only")
{
    const std::vector<double> numbers = {-1.0, -5.0, -3.0};
    REQUIRE_THAT(ComputePositiveMean(numbers), WithinAbs(0.0, kEpsilon));
}

TEST_CASE("ComputePositiveMean: empty vector")
{
    const std::vector<double> numbers = {};
    REQUIRE_THAT(ComputePositiveMean(numbers), WithinAbs(0.0, kEpsilon));
}

TEST_CASE("ComputePositiveMean: zero")
{
    const std::vector<double> numbers = {0.0, 4.0};
    REQUIRE_THAT(ComputePositiveMean(numbers), WithinAbs(4.0, kEpsilon));
}

TEST_CASE("ProcessNumbers: example case 1 (positive values only)")
{
    std::vector<double> numbers = {1.0, 2.0, 3.659512};
    ProcessNumbers(numbers);

    REQUIRE_THAT(numbers[0], WithinAbs(3.2198373, kEpsilon));
    REQUIRE_THAT(numbers[1], WithinAbs(4.2198373, kEpsilon));
    REQUIRE_THAT(numbers[2], WithinAbs(5.8793493, kEpsilon));
}

TEST_CASE("ProcessNumbers: example case 2 (mixed values)")
{
    std::vector<double> numbers = {4.0, 16.0, -30.0, 10.0};
    ProcessNumbers(numbers);

    REQUIRE_THAT(numbers[0], WithinAbs(14.0, kEpsilon));
    REQUIRE_THAT(numbers[1], WithinAbs(26.0, kEpsilon));
    REQUIRE_THAT(numbers[2], WithinAbs(-20.0, kEpsilon));
    REQUIRE_THAT(numbers[3], WithinAbs(20.0, kEpsilon));
}

TEST_CASE("ProcessNumbers: example case 3 (negative values only)")
{
    std::vector<double> numbers = {-1.0004, -703.0, -3.659512, -11.0};
    const std::vector<double> original = numbers;
    ProcessNumbers(numbers);

    for (std::size_t i = 0; i < numbers.size(); ++i)
    {
        REQUIRE_THAT(numbers[i], WithinAbs(original[i], kEpsilon));
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
    REQUIRE_THAT(numbers[0], WithinAbs(10.0, kEpsilon));
}