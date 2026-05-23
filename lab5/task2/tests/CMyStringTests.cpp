#include <catch2/catch_test_macros.hpp>
#include "../CMyString.h"

static const auto HELLO = "Hello";
static const auto WORLD = " World";
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

TEST_CASE("GetStringData always returns buffer that ends with \0", "[getters]")
{
    SECTION("Empty string")
    {
        CMyString str;
        REQUIRE(str.GetStringData()[0] == '\0');
    }

    SECTION("Non-empty string has \0 after last character")
    {
        CMyString str(HELLO);
        REQUIRE(str.GetStringData()[str.GetLength()] == '\0');
    }
}

TEST_CASE("GetLength counts embedded null bytes as part of length", "[getters]")
{
    const char data[] = {'a', '\0', 'b'};
    CMyString str(data, 3);

    REQUIRE(str.GetLength() == 3);
}

TEST_CASE("Clear resets string to empty state", "[clear]")
{
    SECTION("Clear non-empty string")
    {
        CMyString str(HELLO);
        str.Clear();

        REQUIRE(str.GetLength() == 0);
        REQUIRE(str.GetCapacity() == 0);
        REQUIRE(str.GetStringData() != nullptr);
        REQUIRE(str.GetStringData()[0] == '\0');
    }

    SECTION("Clear already empty string does not crash")
    {
        CMyString str;
        REQUIRE_NOTHROW(str.Clear());
        REQUIRE(str.GetLength() == 0);
    }

    SECTION("String is usable after Clear")
    {
        CMyString str(HELLO);
        str.Clear();
        str += CMyString(WORLD);

        REQUIRE(std::string(str.GetStringData()) == WORLD);
    }

    SECTION("Repeated Clear is idempotent")
    {
        CMyString str(HELLO);
        str.Clear();
        str.Clear();

        REQUIRE(str.GetLength() == 0);
    }
}

TEST_CASE("SubString", "[substring]")
{
    CMyString str(HELLO_WORLD);

    SECTION("Normal range from start")
    {
        CMyString sub = str.SubString(0, 5);

        REQUIRE(sub.GetLength() == 5);
        REQUIRE(std::string(sub.GetStringData()) == HELLO);
    }

    SECTION("Normal range in middle")
    {
        CMyString sub = str.SubString(6, 5);

        REQUIRE(std::string(sub.GetStringData()) == "World");
    }

    SECTION("Start at last valid position returns single character")
    {
        CMyString sub = str.SubString(10, 1);

        REQUIRE(sub.GetLength() == 1);
        REQUIRE(sub.GetStringData()[0] == 'd');
    }

    SECTION("Start equals length returns empty string")
    {
        CMyString sub = str.SubString(str.GetLength(), SIZE_MAX);

        REQUIRE(sub.GetLength() == 0);
    }

    SECTION("Start beyond length throws out_of_range")
    {
        REQUIRE_THROWS_AS(str.SubString(str.GetLength() + 1), std::out_of_range);
    }

    SECTION("Length zero returns empty string")
    {
        CMyString sub = str.SubString(0, 0);

        REQUIRE(sub.GetLength() == 0);
    }

    SECTION("Length exceeding available chars is clamped")
    {
        CMyString sub = str.SubString(6, 999);

        REQUIRE(std::string(sub.GetStringData()) == "World");
    }

    SECTION("Default length takes entire tail")
    {
        CMyString sub = str.SubString(6);

        REQUIRE(std::string(sub.GetStringData()) == "World");
    }

    SECTION("Embedded null in substring is preserved")
    {
        const char data[] = {'a', '\0', 'b', 'c'};
        CMyString source(data, 4);
        CMyString sub = source.SubString(0, 3);

        REQUIRE(sub.GetLength() == 3);
        REQUIRE(sub.GetStringData()[1] == '\0');
        REQUIRE(sub.GetStringData()[2] == 'b');
    }

    SECTION("Negative value in SubString throws")
    {
        REQUIRE_THROWS_AS(str.SubString(-10, 3), std::out_of_range);
    }
}

TEST_CASE("Copy assignment operator", "[assignment]")
{
    SECTION("Self-assignment leaves string unchanged")
    {
        CMyString str(HELLO);
        str = str;

        REQUIRE(str.GetLength() == 5);
        REQUIRE(std::string(str.GetStringData()) == HELLO);
    }

    SECTION("Assign non-empty to empty")
    {
        CMyString target;
        CMyString source(HELLO);
        target = source;

        REQUIRE(std::string(target.GetStringData()) == HELLO);
    }

    SECTION("Assign empty to non-empty clears target")
    {
        CMyString target(HELLO);
        CMyString source;
        target = source;

        REQUIRE(target.GetLength() == 0);
    }

    SECTION("Target is independent from source after assignment")
    {
        CMyString source(HELLO);
        CMyString target;
        target = source;
        target[0] = 'X';

        REQUIRE(source.GetStringData()[0] == 'H');
    }
}

TEST_CASE("Move assignment operator", "[assignment]")
{
    SECTION("Self-move-assignment does not corrupt string")
    {
        CMyString str(HELLO);
        str = std::move(str);

        REQUIRE(str.GetLength() == 5);
        REQUIRE(std::string(str.GetStringData()) == HELLO);
    }

    SECTION("Target receives data from donor")
    {
        CMyString source(HELLO);
        CMyString target;
        target = std::move(source);

        REQUIRE(std::string(target.GetStringData()) == HELLO);
    }

    SECTION("Donor is valid empty string after move assignment")
    {
        CMyString source(HELLO);
        CMyString target;
        target = std::move(source);

        REQUIRE(source.GetLength() == 0);
        REQUIRE(source.GetStringData()[0] == '\0');
    }
}

TEST_CASE("operator+=", "[append]")
{
    SECTION("Append non-empty to non-empty")
    {
        CMyString str(HELLO);
        str += CMyString(WORLD);

        REQUIRE(str.GetLength() == 11);
        REQUIRE(std::string(str.GetStringData()) == HELLO_WORLD);
    }

    SECTION("Append empty does not change string or capacity")
    {
        CMyString str(HELLO);
        size_t capacityBefore = str.GetCapacity();
        str += CMyString();

        REQUIRE(str.GetLength() == 5);
        REQUIRE(str.GetCapacity() == capacityBefore);
        REQUIRE(std::string(str.GetStringData()) == HELLO);
    }

    SECTION("Append to empty string")
    {
        CMyString str;
        str += CMyString(HELLO);

        REQUIRE(std::string(str.GetStringData()) == HELLO);
    }

    SECTION("Capacity doubles when exceeded")
    {
        CMyString str("12345");
        REQUIRE(str.GetCapacity() == 5);

        str += CMyString("6");

        REQUIRE(str.GetCapacity() == 10);
    }

    SECTION("Capacity is sufficient when appended string is longer than doubled capacity")
    {
        CMyString str("ab");
        str += CMyString("12345678901234567890");

        REQUIRE(str.GetLength() == 22);
        REQUIRE(str.GetCapacity() >= 22);
    }

    SECTION("Capacity keeps doubling after multiple appends")
    {
        CMyString str("12345");

        str += CMyString("6");
        REQUIRE(str.GetCapacity() == 10);

        str += CMyString("7890123456");
        REQUIRE(str.GetCapacity() == 20);
    }

    SECTION("Append string with embedded null bytes")
    {
        const char data[] = {'x', '\0', 'y'};
        CMyString str(HELLO);
        str += CMyString(data, 3);

        REQUIRE(str.GetLength() == 8);
        REQUIRE(str.GetStringData()[5] == 'x');
        REQUIRE(str.GetStringData()[6] == '\0');
        REQUIRE(str.GetStringData()[7] == 'y');
        REQUIRE(str.GetStringData()[8] == '\0');
    }
}

TEST_CASE("operator[]", "[indexing]")
{
    SECTION("Read valid index")
    {
        CMyString str(HELLO);

        REQUIRE(str[0] == 'H');
        REQUIRE(str[4] == 'o');
    }

    SECTION("Write through non-const reference")
    {
        CMyString str(HELLO);
        str[0] = 'X';

        REQUIRE(str.GetStringData()[0] == 'X');
    }

    SECTION("Last valid index is accessible")
    {
        CMyString str(HELLO);

        REQUIRE(str[4] == 'o');
    }

    SECTION("Index equal to length throws out_of_range")
    {
        CMyString str(HELLO);

        REQUIRE_THROWS_AS(str[5], std::out_of_range);
    }

    SECTION("Any index on empty string throws out_of_range")
    {
        CMyString str;

        REQUIRE_THROWS_AS(str[0], std::out_of_range);
    }

    SECTION("Const version on const string")
    {
        const CMyString str(HELLO);

        REQUIRE(str[0] == 'H');
        REQUIRE_THROWS_AS(str[5], std::out_of_range);
    }
}

TEST_CASE("operator+", "[concatenation]")
{
    SECTION("Two non-empty strings")
    {
        CMyString lhs(HELLO);
        CMyString rhs(WORLD);
        CMyString result = lhs + rhs;

        REQUIRE(std::string(result.GetStringData()) == HELLO_WORLD);
    }

    SECTION("Originals are not modified")
    {
        CMyString lhs(HELLO);
        CMyString rhs(WORLD);
        CMyString result = lhs + rhs;

        REQUIRE(std::string(lhs.GetStringData()) == HELLO);
        REQUIRE(std::string(rhs.GetStringData()) == WORLD);
    }

    SECTION("Empty left operand")
    {
        CMyString result = CMyString() + CMyString(HELLO);

        REQUIRE(std::string(result.GetStringData()) == HELLO);
    }

    SECTION("Empty right operand")
    {
        CMyString result = CMyString(HELLO) + CMyString();

        REQUIRE(std::string(result.GetStringData()) == HELLO);
    }

    SECTION("Both operands empty")
    {
        CMyString result = CMyString() + CMyString();

        REQUIRE(result.GetLength() == 0);
    }

    SECTION("const char* + CMyString")
    {
        CMyString rhs(WORLD);
        CMyString result = HELLO + rhs;

        REQUIRE(std::string(result.GetStringData()) == HELLO_WORLD);
    }

    SECTION("std::string + CMyString")
    {
        CMyString rhs(WORLD);
        CMyString result = std::string(HELLO) + rhs;

        REQUIRE(std::string(result.GetStringData()) == HELLO_WORLD);
    }
}

TEST_CASE("operator==", "[comparison]")
{
    SECTION("Identical strings are equal")
    {
        REQUIRE(CMyString(HELLO) == CMyString(HELLO));
    }

    SECTION("Different content, same length")
    {
        REQUIRE_FALSE(CMyString("Hello") == CMyString("Hxllo"));
    }

    SECTION("Same prefix, different length")
    {
        REQUIRE_FALSE(CMyString("Hello") == CMyString("Hello!"));
    }

    SECTION("Both empty strings are equal")
    {
        REQUIRE(CMyString() == CMyString());
    }

    SECTION("Strings with embedded nulls differing after null byte are not equal")
    {
        const char dataA[] = {'a', '\0', 'b'};
        const char dataB[] = {'a', '\0', 'c'};

        REQUIRE_FALSE(CMyString(dataA, 3) == CMyString(dataB, 3));
    }

    SECTION("Strings with embedded nulls having same bytes are equal")
    {
        const char data[] = {'a', '\0', 'b'};

        REQUIRE(CMyString(data, 3) == CMyString(data, 3));
    }
}

TEST_CASE("operator<=>", "[comparison]")
{
    SECTION("Equal strings")
    {
        REQUIRE((CMyString(HELLO) <=> CMyString(HELLO)) == std::strong_ordering::equal);
    }

    SECTION("Lexicographically less")
    {
        REQUIRE((CMyString("apple") <=> CMyString("banana")) == std::strong_ordering::less);
    }

    SECTION("Lexicographically greater")
    {
        REQUIRE((CMyString("banana") <=> CMyString("apple")) == std::strong_ordering::greater);
    }

    SECTION("Empty string is less than any non-empty string")
    {
        REQUIRE((CMyString() <=> CMyString("a")) == std::strong_ordering::less);
    }

    SECTION("Shorter string with equal prefix is less")
    {
        REQUIRE((CMyString("Hell") <=> CMyString(HELLO)) == std::strong_ordering::less);
    }

    SECTION("Embedded null bytes affect ordering")
    {
        const char earlier[] = {'a', '\0', 'a'};
        const char later[] = {'a', '\0', 'b'};

        REQUIRE((CMyString(earlier, 3) <=> CMyString(later, 3)) == std::strong_ordering::less);
    }

    SECTION("Relational operators work through <=>")
    {
        REQUIRE(CMyString("apple") < CMyString("banana"));
        REQUIRE(CMyString("banana") > CMyString("apple"));
        REQUIRE(CMyString(HELLO) <= CMyString(HELLO));
        REQUIRE(CMyString(HELLO) >= CMyString(HELLO));
    }
}

TEST_CASE("operator<<", "[stream]")
{
    SECTION("Empty string writes nothing to stream")
    {
        std::ostringstream oss;
        oss << CMyString();

        REQUIRE(oss.str().empty());
    }

    SECTION("Non-empty string writes correct content")
    {
        std::ostringstream oss;
        oss << CMyString(HELLO);

        REQUIRE(oss.str() == HELLO);
    }

    SECTION("String with embedded null writes all bytes including null")
    {
        const char data[] = {'a', '\0', 'b'};
        CMyString str(data, 3);
        std::ostringstream oss;
        oss << str;

        std::string result = oss.str();
        REQUIRE(result.size() == 3);
        REQUIRE(result[1] == '\0');
    }
}

TEST_CASE("operator>>", "[stream]")
{
    SECTION("Reads single word")
    {
        std::istringstream iss(HELLO);
        CMyString str;
        iss >> str;

        REQUIRE(std::string(str.GetStringData()) == HELLO);
    }

    SECTION("Reads only first word, stops at whitespace")
    {
        std::istringstream iss(HELLO_WORLD);
        CMyString str;
        iss >> str;

        REQUIRE(std::string(str.GetStringData()) == HELLO);
    }

    SECTION("Reading from stream of only whitespace leaves string empty")
    {
        std::istringstream iss("   ");
        CMyString str;
        iss >> str;

        REQUIRE(str.GetLength() == 0);
    }

    SECTION("Successive reads extract successive words")
    {
        std::istringstream iss(HELLO_WORLD);
        CMyString first, second;
        iss >> first >> second;

        REQUIRE(std::string(first.GetStringData()) == HELLO);
        REQUIRE(std::string(second.GetStringData()) == "World");
    }

    SECTION("Reading into non-empty string replaces its content")
    {
        std::istringstream iss("New");
        CMyString str(HELLO);
        iss >> str;

        REQUIRE(std::string(str.GetStringData()) == "New");
    }
}

TEST_CASE("Capacity management", "[capacity]")
{
    SECTION("Initial capacity equals length after construction")
    {
        CMyString str(HELLO);

        REQUIRE(str.GetCapacity() == str.GetLength());
    }

    SECTION("Append within capacity does not change capacity")
    {
        CMyString str("12345678");
        str += CMyString("90");
        size_t capacityAfterFirstGrow = str.GetCapacity();
        str += CMyString("!");

        REQUIRE(str.GetCapacity() == capacityAfterFirstGrow);
    }

    SECTION("After Clear capacity is zero")
    {
        CMyString str(HELLO);
        str.Clear();

        REQUIRE(str.GetCapacity() == 0);
    }

    SECTION("Capacity after Clear then append equals new length")
    {
        CMyString str(HELLO);
        str.Clear();
        str += CMyString("ab");

        REQUIRE(str.GetCapacity() >= 2);
    }
}