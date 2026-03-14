#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <sstream>
#include <catch2/matchers/catch_matchers.hpp>

#include "../Calculator.h"

TEST_CASE("DeclareVariable creates variable with nan value", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.DeclareVariable("x");
    calculator.Print(out, "x");

    REQUIRE(out.str() == "nan\n");
}

TEST_CASE("DeclareVariable throws if name already exists", "[calculator]")
{
    Calculator calculator;
    calculator.DeclareVariable("x");

    REQUIRE_THROWS_WITH(calculator.DeclareVariable("x"), "Name already exists");
}

TEST_CASE("DeclareVariable throws if name is invalid", "[calculator]")
{
    Calculator calculator;

    REQUIRE_THROWS_WITH(calculator.DeclareVariable("1x"), "Invalid usage");
    REQUIRE_THROWS_WITH(calculator.DeclareVariable(""), "Invalid usage");
    REQUIRE_THROWS_WITH(calculator.DeclareVariable("x y"), "Invalid usage");
}

TEST_CASE("DeclareVariable throws if function with same name exists", "[calculator]")
{
    Calculator calculator;
    calculator.DeclareVariable("x");
    calculator.DeclareFunction("f", "x", std::nullopt, std::nullopt);

    REQUIRE_THROWS_WITH(calculator.DeclareVariable("f"), "Name already exists");
}

TEST_CASE("LetAssign with number sets variable value", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.DeclareVariable("x");
    calculator.LetAssign("x", 42.0);
    calculator.Print(out, "x");

    REQUIRE(out.str() == "42.00\n");
}

TEST_CASE("LetAssign with number auto-declares variable if not exists", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.LetAssign("x", 10.0);
    calculator.Print(out, "x");

    REQUIRE(out.str() == "10.00\n");
}

TEST_CASE("LetAssign with number throws if name belongs to function", "[calculator]")
{
    Calculator calculator;
    calculator.DeclareVariable("x");
    calculator.DeclareFunction("f", "x", std::nullopt, std::nullopt);

    REQUIRE_THROWS_WITH(calculator.LetAssign("f", 42.0), "Invalid usage");
}

TEST_CASE("LetAssign with source copies current value", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.LetAssign("x", 42.0);
    calculator.LetAssign("y", "x");
    calculator.LetAssign("x", 99.0);
    calculator.Print(out, "y");

    REQUIRE(out.str() == "42.00\n");
}

TEST_CASE("LetAssign with source throws if source does not exist", "[calculator]")
{
    Calculator calculator;

    REQUIRE_THROWS_WITH(calculator.LetAssign("x", "y"), "Name does not exist");
}

TEST_CASE("LetAssign with source throws if target is function", "[calculator]")
{
    Calculator calculator;
    calculator.DeclareVariable("x");
    calculator.DeclareVariable("y");
    calculator.DeclareFunction("f", "x", std::nullopt, std::nullopt);

    REQUIRE_THROWS_WITH(calculator.LetAssign("f", "y"), "Invalid usage");
}

TEST_CASE("LetAssign with source can copy from function", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.LetAssign("x", 10.0);
    calculator.LetAssign("y", 4.0);
    calculator.DeclareFunction("f", "x", Operation::Plus, "y");
    calculator.LetAssign("result", "f");
    calculator.LetAssign("x", 99.0);
    calculator.Print(out, "result");

    REQUIRE(out.str() == "14.00\n");
}

TEST_CASE("DeclareFunction throws if name already exists", "[calculator]")
{
    Calculator calculator;
    calculator.DeclareVariable("x");
    calculator.DeclareFunction("f", "x", std::nullopt, std::nullopt);

    REQUIRE_THROWS_WITH(
        calculator.DeclareFunction("f", "x", std::nullopt, std::nullopt),
        "Name already exists");
}

TEST_CASE("DeclareFunction throws if operand does not exist", "[calculator]")
{
    Calculator calculator;

    REQUIRE_THROWS_WITH(
        calculator.DeclareFunction("f", "x", std::nullopt, std::nullopt),
        "Name does not exist");
}

TEST_CASE("DeclareFunction throws if right operand does not exist", "[calculator]")
{
    Calculator calculator;
    calculator.DeclareVariable("x");

    REQUIRE_THROWS_WITH(
        calculator.DeclareFunction("f", "x", Operation::Plus, "y"),
        "Name does not exist");
}

TEST_CASE("Function value reflects current variable value", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.DeclareVariable("x");
    calculator.DeclareFunction("f", "x", std::nullopt, std::nullopt);
    calculator.LetAssign("x", 43.0);
    calculator.Print(out, "x");

    REQUIRE(out.str() == "43.00\n");
}

TEST_CASE("Function applies all binary operations", "[calculator]")
{
    Calculator calculator;
    calculator.LetAssign("x", 10.0);
    calculator.LetAssign("y", 4.0);

    SECTION("addition")
    {
        std::ostringstream out;
        calculator.DeclareFunction("f", "x", Operation::Plus, "y");
        calculator.Print(out, "f");
        REQUIRE(out.str() == "14.00\n");
    }

    SECTION("subtraction")
    {
        std::ostringstream out;
        calculator.DeclareFunction("f", "x", Operation::Minus, "y");
        calculator.Print(out, "f");
        REQUIRE(out.str() == "6.00\n");
    }

    SECTION("multiplication")
    {
        std::ostringstream out;
        calculator.DeclareFunction("f", "x", Operation::Multiply, "y");
        calculator.Print(out, "f");
        REQUIRE(out.str() == "40.00\n");
    }

    SECTION("division")
    {
        std::ostringstream out;
        calculator.DeclareFunction("f", "x", Operation::Divide, "y");
        calculator.Print(out, "f");
        REQUIRE(out.str() == "2.50\n");
    }
}

TEST_CASE("Function returns nan on division by zero", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.LetAssign("x", 10.0);
    calculator.LetAssign("y", 0.0);
    calculator.DeclareFunction("f", "x", Operation::Divide, "y");
    calculator.Print(out, "f");

    REQUIRE(out.str() == "nan\n");
}

TEST_CASE("Function can depend on another function", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.LetAssign("x", 10.0);
    calculator.LetAssign("y", 4.0);
    calculator.LetAssign("z", 2.0);
    calculator.DeclareFunction("sum", "x", Operation::Plus, "y");
    calculator.DeclareFunction("result", "sum", Operation::Divide, "z");
    calculator.Print(out, "result");

    REQUIRE(out.str() == "7.00\n");
}

TEST_CASE("Print throws if identifier does not exist", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    REQUIRE_THROWS_WITH(calculator.Print(out, "x"), "Name does not exist");
}

TEST_CASE("PrintVars outputs variables in alphabetical order", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.LetAssign("z", 3.0);
    calculator.LetAssign("a", 1.0);
    calculator.LetAssign("m", 2.0);
    calculator.PrintVars(out);

    REQUIRE(out.str() == "a:1.00\nm:2.00\nz:3.00\n");
}

TEST_CASE("PrintVars outputs nothing if no variables declared", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.PrintVars(out);

    REQUIRE(out.str().empty());
}

TEST_CASE("PrintVars does not include functions", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.LetAssign("x", 1.0);
    calculator.DeclareFunction("f", "x", std::nullopt, std::nullopt);
    calculator.PrintVars(out);

    REQUIRE(out.str() == "x:1.00\n");
}

TEST_CASE("PrintFns outputs functions in alphabetical order", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.LetAssign("x", 3.0);
    calculator.DeclareFunction("z", "x", std::nullopt, std::nullopt);
    calculator.DeclareFunction("a", "x", std::nullopt, std::nullopt);
    calculator.PrintFns(out);

    REQUIRE(out.str() == "a:3.00\nz:3.00\n");
}

TEST_CASE("PrintFns outputs nothing if no functions declared", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.PrintFns(out);

    REQUIRE(out.str().empty());
}

TEST_CASE("Fibonacci sequence scenario from spec", "[calculator]")
{
    Calculator calculator;
    std::ostringstream out;

    calculator.LetAssign("v0", 0.0);
    calculator.LetAssign("v1", 1.0);
    calculator.DeclareFunction("fib0", "v0", std::nullopt, std::nullopt);
    calculator.DeclareFunction("fib1", "v1", std::nullopt, std::nullopt);
    calculator.DeclareFunction("fib2", "fib1", Operation::Plus, "fib0");
    calculator.DeclareFunction("fib3", "fib2", Operation::Plus, "fib1");
    calculator.DeclareFunction("fib4", "fib3", Operation::Plus, "fib2");
    calculator.DeclareFunction("fib5", "fib4", Operation::Plus, "fib3");
    calculator.DeclareFunction("fib6", "fib5", Operation::Plus, "fib4");
    calculator.PrintFns(out);

    REQUIRE(out.str() ==
        "fib0:0.00\n"
        "fib1:1.00\n"
        "fib2:1.00\n"
        "fib3:2.00\n"
        "fib4:3.00\n"
        "fib5:5.00\n"
        "fib6:8.00\n");
}