#include <cmath>
#include <catch2/catch_test_macros.hpp>

#include "../Variable.h"

TEST_CASE("Variable initial value is nan", "[variable]")
{
    const Variable variable;
    REQUIRE(std::isnan(variable.GetValue()));
}

TEST_CASE("Variable stores value after SetValue", "[variable]")
{
    Variable variable;

    SECTION("positive number")
    {
        variable.SetValue(42.0);
        REQUIRE(variable.GetValue() == 42.0);
    }

    SECTION("negative number")
    {
        variable.SetValue(-3.14);
        REQUIRE(variable.GetValue() == -3.14);
    }

    SECTION("zero")
    {
        variable.SetValue(0.0);
        REQUIRE(variable.GetValue() == 0.0);
    }
}

TEST_CASE("Variable value can be overwritten", "[variable]")
{
    Variable variable;
    variable.SetValue(1.0);
    variable.SetValue(2.0);
    REQUIRE(variable.GetValue() == 2.0);
}

TEST_CASE("Variable can be set back to nan", "[variable]")
{
    Variable variable;
    variable.SetValue(42.0);
    variable.SetValue(std::numeric_limits<double>::quiet_NaN());
    REQUIRE(std::isnan(variable.GetValue()));
}