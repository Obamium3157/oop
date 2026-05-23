#include <catch2/catch_test_macros.hpp>
#include "../CMyString.h"

static const auto HELLO = "Hello";
static const auto WORLD = "World";
static const auto HELLO_WORLD = "Hello World";

TEST_CASE("Default constructor creates empty valid string", "[constructor]")
{
    CMyString str;

    REQUIRE(str.GetLength() == 0);
    REQUIRE(str.GetCapacity() == 0);
    REQUIRE(str.GetStringData() != nullptr);
    REQUIRE(str.GetStringData()[0] == '\0');
}

TEST_CASE("Constructor from null pointer throws", "[constructor]")
{
    REQUIRE_THROWS_AS(CMyString(nullptr), std::invalid_argument);
}

TEST_CASE("Constructor from empty CString", "[constructor]")
{
    CMyString str("");

    REQUIRE(str.GetLength() == 0);
    REQUIRE(str.GetStringData() != nullptr);
    REQUIRE(str.GetStringData()[0] == '\0');
}

TEST_CASE("Constructor from C-string copies data correctly", "[constructor]")
{
    CMyString str(HELLO);

    REQUIRE(str.GetLength() == 5);
    REQUIRE(str.GetCapacity() == 5);
    REQUIRE(std::string(str.GetStringData()) == HELLO);
    REQUIRE(str.GetStringData()[5] == '\0');
}

TEST_CASE("Constructor from char array with length", "[constructor]")
{
    SECTION("Zero length produces empty string")
    {
        CMyString str(HELLO, 0);

        REQUIRE(str.GetLength() == 0);
        REQUIRE(str.GetStringData()[0] == '\0');
    }

    SECTION("Length smaller than full C-string takes only requested bytes")
    {
        CMyString str(HELLO, 3);

        REQUIRE(str.GetLength() == 3);
        REQUIRE(str.GetStringData()[0] == 'H');
        REQUIRE(str.GetStringData()[1] == 'e');
        REQUIRE(str.GetStringData()[2] == 'l');
        REQUIRE(str.GetStringData()[3] == '\0');
    }

    SECTION("Embedded null bytes are preserved")
    {
        const char data[] = {'a', '\0', 'b'};
        CMyString str(data, 3);

        REQUIRE(str.GetLength() == 3);
        REQUIRE(str.GetStringData()[0] == 'a');
        REQUIRE(str.GetStringData()[1] == '\0');
        REQUIRE(str.GetStringData()[2] == 'b');
        REQUIRE(str.GetStringData()[3] == '\0');
    }

    SECTION("Null pointer with zero length throws")
    {
        REQUIRE_THROWS_AS(CMyString(nullptr, 0), std::invalid_argument);
    }
}

TEST_CASE("Copy constructor", "[constructor]")
{
    SECTION("Copy of empty string is valid empty string")
    {
        CMyString original;
        CMyString copy(original);

        REQUIRE(copy.GetLength() == 0);
        REQUIRE(copy.GetStringData() != nullptr);
        REQUIRE(copy.GetStringData()[0] == '\0');
    }

    SECTION("Copy has same content as original")
    {
        CMyString original(HELLO);
        CMyString copy(original);

        REQUIRE(copy.GetLength() == original.GetLength());
        REQUIRE(std::string(copy.GetStringData()) == HELLO);
    }

    SECTION("Copy is independent from original")
    {
        CMyString original(HELLO);
        CMyString copy(original);
        copy[0] = 'X';

        REQUIRE(original.GetStringData()[0] == 'H');
    }

    SECTION("Copy of moved-from string does not crash")
    {
        CMyString original(HELLO);
        CMyString moved(std::move(original));
        CMyString copyOfDonor(original);

        REQUIRE(copyOfDonor.GetLength() == 0);
        REQUIRE(copyOfDonor.GetStringData()[0] == '\0');
    }
}

TEST_CASE("Move constructor", "[constructor]")
{
    SECTION("Moved string receives original data")
    {
        CMyString original(HELLO);
        CMyString moved(std::move(original));

        REQUIRE(moved.GetLength() == 5);
        REQUIRE(std::string(moved.GetStringData()) == HELLO);
    }

    SECTION("Donor after move is valid empty string")
    {
        CMyString original(HELLO);
        CMyString moved(std::move(original));

        REQUIRE(original.GetLength() == 0);
        REQUIRE(original.GetCapacity() == 0);
        REQUIRE(original.GetStringData() != nullptr);
        REQUIRE(original.GetStringData()[0] == '\0');
    }

    SECTION("Donor after move can be used with +=")
    {
        CMyString original(HELLO);
        CMyString moved(std::move(original));

        REQUIRE_NOTHROW(original += CMyString(WORLD));
        REQUIRE(std::string(original.GetStringData()) == WORLD);
    }
}

TEST_CASE("Constructor from std::string", "[constructor]")
{
    SECTION("Empty std::string")
    {
        CMyString str(std::string(""));

        REQUIRE(str.GetLength() == 0);
        REQUIRE(str.GetStringData()[0] == '\0');
    }

    SECTION("Non-empty std::string")
    {
        CMyString str{std::string(HELLO)};

        REQUIRE(str.GetLength() == 5);
        REQUIRE(std::string(str.GetStringData()) == HELLO);
    }

    SECTION("std::string with embedded null bytes")
    {
        std::string source(3, '\0');
        source[0] = 'a';
        source[2] = 'b';
        CMyString str(source);

        REQUIRE(str.GetLength() == 3);
        REQUIRE(str.GetStringData()[0] == 'a');
        REQUIRE(str.GetStringData()[1] == '\0');
        REQUIRE(str.GetStringData()[2] == 'b');
    }
}