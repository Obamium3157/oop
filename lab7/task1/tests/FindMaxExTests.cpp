#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "../FindMaxEx.h"
#include "../Athlete.h"


namespace
{
    std::vector<Athlete> const sampleAthletes =
    {
        {"Athlete 1", 182.5, 78.0},
        {"Athlete 2", 195.0, 92.5},
        {"Athlete 3", 170.0, 110.3},
        {"Athlete 4", 188.0, 85.0},
    };

    const auto less = [](const auto lhs, const auto rhs)
        noexcept
    {
        return lhs < rhs;
    };

    const auto greater = [](const auto lhs, const auto rhs)
        noexcept
    {
        return lhs > rhs;
    };

    const auto stringLess = [](const std::string& lhs, const std::string& rhs)
        noexcept
    {
        return lhs < rhs;
    };

    const auto stringGreater = [](const std::string& lhs, const std::string& rhs)
        noexcept
    {
        return lhs > rhs;
    };
}

TEST_CASE("FindMaxEx returns false for empty array", "[FindMaxEx]")
{
    std::vector<Athlete> const emptyAthletes;
    Athlete sentinel{"sentinel", 999.0, 999.0};

    bool const found = FindMaxEx(emptyAthletes.begin(), emptyAthletes.end(), sentinel, LessByHeight{});

    REQUIRE_FALSE(found);

    SECTION("maxValue is not modified on empty input")
    {
        CHECK(sentinel.fullName == "sentinel");
        CHECK(sentinel.heightCm == Catch::Approx(999.0));
        CHECK(sentinel.weightKg == Catch::Approx(999.0));
    }
}

TEST_CASE("FindMaxEx returns true and the only element for single-element array", "[FindMaxEx]")
{
    std::vector<Athlete> const oneAthlete = {{"Only One", 180.0, 80.0}};
    Athlete result{};

    bool const found = FindMaxEx(oneAthlete.begin(), oneAthlete.end(), result, LessByHeight{});

    REQUIRE(found);
    CHECK(result.fullName == "Only One");
    CHECK(result.heightCm == Catch::Approx(180.0));
}

TEST_CASE("FindMaxEx finds tallest athlete", "[FindMaxEx][height]")
{
    Athlete tallest{};

    bool const found = FindMaxEx(sampleAthletes.begin(), sampleAthletes.end(), tallest, LessByHeight{});

    REQUIRE(found);
    CHECK(tallest.fullName == "Athlete 2");
    CHECK(tallest.heightCm == Catch::Approx(195.0));
}

TEST_CASE("FindMaxEx finds heaviest athlete", "[FindMaxEx][weight]")
{
    Athlete heaviest{};

    bool const found = FindMaxEx(sampleAthletes.begin(), sampleAthletes.end(), heaviest, LessByWeight{});

    REQUIRE(found);
    CHECK(heaviest.fullName == "Athlete 3");
    CHECK(heaviest.weightKg == Catch::Approx(110.3));
}

TEST_CASE("FindMaxEx returns first element when all are equal", "[FindMaxEx]")
{
    std::vector<Athlete> const equalAthletes =
    {
        {"uno", 180.0, 80.0},
        {"dos", 180.0, 80.0},
        {"tres", 180.0, 80.0},
    };
    Athlete result{};

    FindMaxEx(equalAthletes.begin(), equalAthletes.end(), result, LessByHeight{});

    CHECK(result.fullName == "uno");
}

TEST_CASE("FindMaxEx works with lambda comparator", "[FindMaxEx][lambda]")
{
    Athlete shortest{};

    auto const greaterHeight = [](Athlete const& left, Athlete const& right)
        noexcept
    {
        return left.heightCm > right.heightCm;
    };

    bool const found = FindMaxEx(sampleAthletes.begin(), sampleAthletes.end(), shortest, greaterHeight);

    REQUIRE(found);
    CHECK(shortest.fullName == "Athlete 3");
    CHECK(shortest.heightCm == Catch::Approx(170.0));
}

TEST_CASE("FindMaxEx works with primitive int type", "[FindMaxEx][generic]")
{
    std::vector const numbers = {3, 1, 4, 1, 5, 9, 2, 6};
    int maxNumber{};

    bool const found = FindMaxEx(numbers.begin(), numbers.end(), maxNumber, less);

    REQUIRE(found);
    CHECK(maxNumber == 9);
}

TEST_CASE("FindMaxEx finds max in negative int array", "[FindMaxEx][generic]")
{
    std::vector const numbers = {-5, -1, -3, -10, -2};
    int maxNumber{};

    bool const found = FindMaxEx(numbers.begin(), numbers.end(), maxNumber, less);

    REQUIRE(found);
    CHECK(maxNumber == -1);
}

TEST_CASE("FindMaxEx finds max in single-element int array", "[FindMaxEx][generic]")
{
    std::vector const numbers = {42};
    int maxNumber{};

    bool const found = FindMaxEx(numbers.begin(), numbers.end(), maxNumber, less);

    REQUIRE(found);
    CHECK(maxNumber == 42);
}

TEST_CASE("FindMaxEx returns false for empty int array", "[FindMaxEx][generic]")
{
    std::vector<int> const numbers;
    int sentinel = -999;

    bool const found = FindMaxEx(numbers.begin(), numbers.end(), sentinel, less);

    REQUIRE_FALSE(found);
    CHECK(sentinel == -999);
}

TEST_CASE("FindMaxEx finds min in int array via reversed comparator", "[FindMaxEx][generic]")
{
    std::vector const numbers = {3, 1, 4, 1, 5, 9, 2, 6};
    int minNumber{};

    bool const found = FindMaxEx(numbers.begin(), numbers.end(), minNumber, greater);

    REQUIRE(found);
    CHECK(minNumber == 1);
}

TEST_CASE("FindMaxEx finds max in double array", "[FindMaxEx][generic]")
{
    std::vector const values = {1.5, 3.14, 2.71, 0.577};
    double maxValue{};

    bool const found = FindMaxEx(values.begin(), values.end(), maxValue, less);

    REQUIRE(found);
    CHECK(maxValue == Catch::Approx(3.14));
}

TEST_CASE("FindMaxEx finds lexicographically greatest string", "[FindMaxEx][string]")
{
    std::vector<std::string> const words = {"banana", "apple", "cherry", "date"};
    std::string maxWord;

    bool const found = FindMaxEx(words.begin(), words.end(), maxWord, stringLess);

    REQUIRE(found);
    CHECK(maxWord == "date");
}

TEST_CASE("FindMaxEx finds lexicographically smallest string via reversed comparator", "[FindMaxEx][string]")
{
    std::vector<std::string> const words = {"banana", "apple", "cherry", "date"};
    std::string minWord;

    bool const found = FindMaxEx(words.begin(), words.end(), minWord, stringGreater);

    REQUIRE(found);
    CHECK(minWord == "apple");
}

TEST_CASE("FindMaxEx finds longest string via length comparator", "[FindMaxEx][string]")
{
    std::vector<std::string> const words = {"cat", "elephant", "ox", "hippopotamus"};
    std::string longest;

    auto const shorterThan = [](std::string const& left, std::string const& right)
        noexcept
    {
        return left.size() < right.size();
    };

    bool const found = FindMaxEx(words.begin(), words.end(), longest, shorterThan);

    REQUIRE(found);
    CHECK(longest == "hippopotamus");
}

TEST_CASE("FindMaxEx returns false for empty string array", "[FindMaxEx][string]")
{
    std::vector<std::string> const words;
    std::string sentinel = "unchanged";

    bool const found = FindMaxEx(words.begin(), words.end(), sentinel, stringLess);

    REQUIRE_FALSE(found);
    CHECK(sentinel == "unchanged");
}

TEST_CASE("FindMaxEx returns first element when all strings are equal", "[FindMaxEx][string]")
{
    std::vector<std::string> const words = {"same", "same", "same"};
    std::string result;

    FindMaxEx(words.begin(), words.end(), result, stringLess);

    CHECK(result == "same");
}
