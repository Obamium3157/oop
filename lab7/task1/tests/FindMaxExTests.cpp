#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "../FindMaxEx.h"
#include "../Athlete.h"

static std::vector<Athlete> const sampleAthletes =
{
    {"Athlete 1", 182.5, 78.0},
    {"Athlete 2", 195.0, 92.5},
    {"Athlete 3", 170.0, 110.3},
    {"Athlete 4", 188.0, 85.0},
};


TEST_CASE("FindMaxEx returns false for empty array", "[FindMaxEx]")
{
    std::vector<Athlete> const emptyAthletes;
    Athlete sentinel{"sentinel", 999.0, 999.0};

    bool const found = FindMaxEx(emptyAthletes, sentinel, LessByHeight{});

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

    bool const found = FindMaxEx(oneAthlete, result, LessByHeight{});

    REQUIRE(found);
    CHECK(result.fullName == "Only One");
    CHECK(result.heightCm == Catch::Approx(180.0));
}

TEST_CASE("FindMaxEx finds tallest athlete", "[FindMaxEx][height]")
{
    Athlete tallest{};

    bool const found = FindMaxEx(sampleAthletes, tallest, LessByHeight{});

    REQUIRE(found);
    CHECK(tallest.fullName == "Athlete 2");
    CHECK(tallest.heightCm == Catch::Approx(195.0));
}

TEST_CASE("FindMaxEx finds heaviest athlete", "[FindMaxEx][weight]")
{
    Athlete heaviest{};

    bool const found = FindMaxEx(sampleAthletes, heaviest, LessByWeight{});

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

    FindMaxEx(equalAthletes, result, LessByHeight{});

    CHECK(result.fullName == "uno");
}

TEST_CASE("FindMaxEx works with lambda comparator", "[FindMaxEx][lambda]")
{
    Athlete shortest{};

    auto const greaterHeight = [](Athlete const& left, Athlete const& right)
    {
        return left.heightCm > right.heightCm;
    };

    bool const found = FindMaxEx(sampleAthletes, shortest, greaterHeight);

    REQUIRE(found);
    CHECK(shortest.fullName == "Athlete 3");
    CHECK(shortest.heightCm == Catch::Approx(170.0));
}

TEST_CASE("FindMaxEx works with primitive int type", "[FindMaxEx][generic]")
{
    std::vector const numbers = {3, 1, 4, 1, 5, 9, 2, 6};
    int maxNumber{};

    bool const found = FindMaxEx(numbers, maxNumber, std::less<int>{});

    REQUIRE(found);
    CHECK(maxNumber == 9);
}
