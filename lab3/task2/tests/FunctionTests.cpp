#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <unordered_map>

#include "../Function.h"


static Function::IdentifierResolver makeResolver(const std::unordered_map<std::string, double>& values)
{
    return [values](const std::string& name) -> double
    {
        const auto it = values.find(name);
        if (it == values.end())
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return it->second;
    };
}

TEST_CASE("Function with single operand returns its value", "[function]")
{
    const auto resolver = makeResolver({{"x", 42.0}});
    const Function function(resolver, "x", std::nullopt, std::nullopt);
    REQUIRE(function.GetValue() == 42.0);
}

TEST_CASE("Function with undefined operand returns nan", "[function]")
{
    const auto resolver = makeResolver({});
    const Function function(resolver, "x", std::nullopt, std::nullopt);
    REQUIRE(std::isnan(function.GetValue()));
}

TEST_CASE("Function applies binary operations correctly", "[function]")
{
    const auto resolver = makeResolver({{"x", 10.0}, {"y", 4.0}});

    SECTION("addition")
    {
        const Function function(resolver, "x", Operation::Plus, "y");
        REQUIRE(function.GetValue() == 14.0);
    }

    SECTION("subtraction")
    {
        const Function function(resolver, "x", Operation::Minus, "y");
        REQUIRE(function.GetValue() == 6.0);
    }

    SECTION("multiplication")
    {
        const Function function(resolver, "x", Operation::Multiply, "y");
        REQUIRE(function.GetValue() == 40.0);
    }

    SECTION("division")
    {
        const Function function(resolver, "x", Operation::Divide, "y");
        REQUIRE(function.GetValue() == 2.5);
    }
}

TEST_CASE("Function returns nan if any operand is nan", "[function]")
{
    SECTION("left operand is nan")
    {
        const auto resolver = makeResolver({{"y", 4.0}});
        const Function function(resolver, "x", Operation::Plus, "y");
        REQUIRE(std::isnan(function.GetValue()));
    }

    SECTION("right operand is nan")
    {
        const auto resolver = makeResolver({{"x", 4.0}});
        const Function function(resolver, "x", Operation::Plus, "y");
        REQUIRE(std::isnan(function.GetValue()));
    }
}

TEST_CASE("Function returns nan on division by zero", "[function]")
{
    const auto resolver = makeResolver({{"x", 10.0}, {"y", 0.0}});
    const Function function(resolver, "x", Operation::Divide, "y");
    REQUIRE(std::isnan(function.GetValue()));
}

TEST_CASE("Function reflects current value of operand", "[function]")
{
    double xValue = 1.0;
    const auto resolver = [&xValue](const std::string& name) -> double
    {
        if (name == "x") return xValue;
        return std::numeric_limits<double>::quiet_NaN();
    };

    const Function function(resolver, "x", std::nullopt, std::nullopt);
    REQUIRE(function.GetValue() == 1.0);

    xValue = 99.0;
    REQUIRE(function.GetValue() == 99.0);
}