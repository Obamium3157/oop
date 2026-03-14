#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "../Function.h"
#include "../Variable.h"

TEST_CASE("Function with single operand returns its value", "[function]")
{
    Variable x;
    x.SetValue(42.0);
    const Function function(&x, std::nullopt, nullptr);
    REQUIRE(function.GetValue() == 42.0);
}

TEST_CASE("Function with undefined operand returns nan", "[function]")
{
    Variable x;
    const Function function(&x, std::nullopt, nullptr);
    REQUIRE(std::isnan(function.GetValue()));
}

TEST_CASE("Function applies binary operations correctly", "[function]")
{
    Variable x;
    Variable y;
    x.SetValue(10.0);
    y.SetValue(4.0);

    SECTION("addition")
    {
        const Function function(&x, Operation::Plus, &y);
        REQUIRE(function.GetValue() == 14.0);
    }

    SECTION("subtraction")
    {
        const Function function(&x, Operation::Minus, &y);
        REQUIRE(function.GetValue() == 6.0);
    }

    SECTION("multiplication")
    {
        const Function function(&x, Operation::Multiply, &y);
        REQUIRE(function.GetValue() == 40.0);
    }

    SECTION("division")
    {
        const Function function(&x, Operation::Divide, &y);
        REQUIRE(function.GetValue() == 2.5);
    }
}

TEST_CASE("Function returns nan if any operand is nan", "[function]")
{
    Variable x;
    Variable y;

    SECTION("left operand is nan")
    {
        y.SetValue(4.0);
        const Function function(&x, Operation::Plus, &y);
        REQUIRE(std::isnan(function.GetValue()));
    }

    SECTION("right operand is nan")
    {
        x.SetValue(4.0);
        const Function function(&x, Operation::Plus, &y);
        REQUIRE(std::isnan(function.GetValue()));
    }
}

TEST_CASE("Function returns nan on division by zero", "[function]")
{
    Variable x;
    Variable y;
    x.SetValue(10.0);
    y.SetValue(0.0);
    const Function function(&x, Operation::Divide, &y);
    REQUIRE(std::isnan(function.GetValue()));
}

TEST_CASE("Function reflects current value of operand", "[function]")
{
    Variable x;
    x.SetValue(1.0);
    const Function function(&x, std::nullopt, nullptr);
    REQUIRE(function.GetValue() == 1.0);

    x.SetValue(99.0);
    REQUIRE(function.GetValue() == 99.0);
}