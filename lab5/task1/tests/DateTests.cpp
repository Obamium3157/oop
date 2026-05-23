#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <stdexcept>
#include "../CDate.h"

TEST_CASE("Default constructor creates 01.01.1970", "[constructor]")
{
    const CDate date;
    CHECK(date.GetDay() == 1);
    CHECK(date.GetMonth() == Month::JANUARY);
    CHECK(date.GetYear() == 1970);
}

TEST_CASE("Timestamp constructor", "[constructor]")
{
    SECTION("timestamp 0 = 01.01.1970")
    {
        CDate date(0);
        CHECK(date.GetDay() == 1);
        CHECK(date.GetMonth() == Month::JANUARY);
        CHECK(date.GetYear() == 1970);
    }
    SECTION("timestamp 2 = 03.01.1970")
    {
        CDate date(2);
        CHECK(date.GetDay() == 3);
        CHECK(date.GetMonth() == Month::JANUARY);
        CHECK(date.GetYear() == 1970);
    }
    SECTION("timestamp 32 = 02.02.1970")
    {
        CDate date(32);
        CHECK(date.GetDay() == 2);
        CHECK(date.GetMonth() == Month::FEBRUARY);
        CHECK(date.GetYear() == 1970);
    }
    SECTION("timestamp out of valid range throws")
    {
        CHECK_THROWS_AS(CDate(std::numeric_limits<unsigned>::max()), std::invalid_argument);
    }
}

TEST_CASE("Day/month/year constructor", "[constructor]")
{
    SECTION("valid date")
    {
        CHECK_NOTHROW(CDate(15, Month::JUNE, 2000));
    }
    SECTION("maximum valid date: 31.12.9999")
    {
        CHECK_NOTHROW(CDate(31, Month::DECEMBER, 9999));
    }
    SECTION("invalid day throws")
    {
        CHECK_THROWS_AS(CDate(32, Month::JANUARY, 2000), std::invalid_argument);
    }
    SECTION("zero day throws")
    {
        CHECK_THROWS_AS(CDate(0, Month::JANUARY, 2000), std::invalid_argument);
    }
    SECTION("invalid month throws")
    {
        CHECK_THROWS_AS(CDate(1, static_cast<Month>(13), 2000), std::invalid_argument);
    }
    SECTION("year before 1970 throws")
    {
        CHECK_THROWS_AS(CDate(1, Month::JANUARY, 1969), std::invalid_argument);
    }
    SECTION("year after 9999 throws")
    {
        CHECK_THROWS_AS(CDate(1, Month::JANUARY, 10000), std::invalid_argument);
    }
}

TEST_CASE("Leap years", "[leap]")
{
    SECTION("Regular leap year (divisible by 4, not by 100)")
    {
        CHECK_NOTHROW(CDate(29, Month::FEBRUARY, 1972));
        CHECK_NOTHROW(CDate(29, Month::FEBRUARY, 2004));
        CHECK_NOTHROW(CDate(29, Month::FEBRUARY, 2024));
    }

    SECTION("Regular non-leap year (not divisible by 4)")
    {
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 2001), std::invalid_argument);
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 2002), std::invalid_argument);
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 2003), std::invalid_argument);
    }

    SECTION("Century year is not a leap year (divisible by 100, not by 400)")
    {
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 2100), std::invalid_argument);
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 2200), std::invalid_argument);
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 2300), std::invalid_argument);
    }

    SECTION("400-year century is a leap year")
    {
        CHECK_NOTHROW(CDate(29, Month::FEBRUARY, 2000));
        CHECK_NOTHROW(CDate(29, Month::FEBRUARY, 2400));
    }

    SECTION("Year 2000 +- 1 (400 year boundary)")
    {
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 1999), std::invalid_argument);
        CHECK_NOTHROW(CDate(29, Month::FEBRUARY, 2000));
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 2001), std::invalid_argument);
    }

    SECTION("Year 2100 (100-year boundary)")
    {
        CHECK_NOTHROW(CDate(29, Month::FEBRUARY, 2096));
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 2097), std::invalid_argument);
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 2100), std::invalid_argument);
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 2101), std::invalid_argument);
        CHECK_NOTHROW(CDate(29, Month::FEBRUARY, 2104));
    }

    SECTION("Year 1972 +- 1")
    {
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 1971), std::invalid_argument);
        CHECK_NOTHROW(CDate(29, Month::FEBRUARY, 1972));
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 1973), std::invalid_argument);
    }

    SECTION("Boundary aof valid date range")
    {
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 1970), std::invalid_argument);
        CHECK_NOTHROW(CDate(29, Month::FEBRUARY, 1972));
        CHECK_NOTHROW(CDate(29, Month::FEBRUARY, 9996));
        CHECK_THROWS_AS(CDate(29, Month::FEBRUARY, 9999), std::invalid_argument);
    }

    SECTION("February 28 is always valid")
    {
        CHECK_NOTHROW(CDate(28, Month::FEBRUARY, 2000));
        CHECK_NOTHROW(CDate(28, Month::FEBRUARY, 2001));
        CHECK_NOTHROW(CDate(28, Month::FEBRUARY, 2100));
    }

    SECTION("Increment across February in leap year")
    {
        CDate date(28, Month::FEBRUARY, 2000);
        ++date;
        CHECK(date.GetDay() == 29);
        CHECK(date.GetMonth() == Month::FEBRUARY);
        ++date;
        CHECK(date.GetDay() == 1);
        CHECK(date.GetMonth() == Month::MARCH);
    }

    SECTION("Increment across February in non-leap year")
    {
        CDate date(28, Month::FEBRUARY, 2001);
        ++date;
        CHECK(date.GetDay() == 1);
        CHECK(date.GetMonth() == Month::MARCH);
    }

    SECTION("GetDay, GetMonth, GetYear are correct for February 29")
    {
        CDate date(29, Month::FEBRUARY, 2000);
        CHECK(date.GetDay() == 29);
        CHECK(date.GetMonth() == Month::FEBRUARY);
        CHECK(date.GetYear() == 2000);
    }
}

TEST_CASE("Weekday", "[weekday]")
{
    SECTION("01.01.1970 is Thursday")
    {
        CHECK(CDate().GetWeekDay() == WeekDay::THURSDAY);
    }
    SECTION("02.01.1970 is Friday")
    {
        CDate date;
        ++date;
        CHECK(date.GetWeekDay() == WeekDay::FRIDAY);
    }
    SECTION("01.01.2024 is Monday")
    {
        CHECK(CDate(1, Month::JANUARY, 2024).GetWeekDay() == WeekDay::MONDAY);
    }
}

TEST_CASE("Prefix ++", "[increment]")
{
    SECTION("day++ = day + 1", "[increment]")
    {
        CDate date;
        CDate other = ++date;
        CHECK(other == date);
        CHECK(date.GetDay() == 2);
        CHECK(date.GetMonth() == Month::JANUARY);
        CHECK(date.GetYear() == 1970);
    }
    SECTION("crossing month boundary (non-leap year)")
    {
        CDate date(28, Month::FEBRUARY, 2001);
        ++date;
        CHECK(date.GetDay() == 1);
        CHECK(date.GetMonth() == Month::MARCH);
    }
    SECTION("crossing end of February (leap year)")
    {
        CDate date(28, Month::FEBRUARY, 2000);
        ++date;
        CHECK(date.GetDay() == 29);
        CHECK(date.GetMonth() == Month::FEBRUARY);
    }
    SECTION("crossing year boundary")
    {
        CDate date(31, Month::DECEMBER, 2000);
        ++date;
        CHECK(date.GetDay() == 1);
        CHECK(date.GetMonth() == Month::JANUARY);
        CHECK(date.GetYear() == 2001);
    }
    SECTION("going past maximum throws")
    {
        CDate date(31, Month::DECEMBER, 9999);
        CHECK_THROWS_AS(++date, std::out_of_range);
    }
}

TEST_CASE("Postfix ++", "[increment]")
{
    CDate date(31, Month::JANUARY, 2000);
    CDate old = date++;
    CHECK(old.GetDay() == 31);
    CHECK(old.GetMonth() == Month::JANUARY);
    CHECK(date.GetDay() == 1);
    CHECK(date.GetMonth() == Month::FEBRUARY);
}

TEST_CASE("Prefix --", "[decrement]")
{
    SECTION("--day = day - 1")
    {
        CDate date(2, Month::JANUARY, 1970);
        CDate other = --date;
        CHECK(other == date);
        CHECK(date.GetDay() == 1);
        CHECK(date.GetMonth() == Month::JANUARY);
    }
    SECTION("crossing month boundary")
    {
        CDate date(1, Month::MARCH, 2001);
        --date;
        CHECK(date.GetDay() == 28);
        CHECK(date.GetMonth() == Month::FEBRUARY);
    }
    SECTION("crossing start of March (leap year)")
    {
        CDate date(1, Month::MARCH, 2000);
        --date;
        CHECK(date.GetDay() == 29);
        CHECK(date.GetMonth() == Month::FEBRUARY);
    }
    SECTION("decrement from epoch throws")
    {
        CDate date(1, Month::JANUARY, 1970);
        CHECK_THROWS_AS(--date, std::out_of_range);
    }
}

TEST_CASE("Postfix --", "[decrement]")
{
    CDate date(1, Month::MARCH, 2001);
    CDate old = date--;
    CHECK(old.GetDay() == 1);
    CHECK(old.GetMonth() == Month::MARCH);
    CHECK(date.GetDay() == 28);
    CHECK(date.GetMonth() == Month::FEBRUARY);
}

TEST_CASE("Adding days (+)", "[arithmetic]")
{
    SECTION("28.02.2010 + 3 = 03.03.2010")
    {
        CDate result = CDate(28, Month::FEBRUARY, 2010) + 3;
        CHECK(result.GetDay() == 3);
        CHECK(result.GetMonth() == Month::MARCH);
        CHECK(result.GetYear() == 2010);
    }
    SECTION("3 + date = date + 3 (commutativity)")
    {
        CDate date(28, Month::FEBRUARY, 2010);
        CHECK((date + 3) == (3 + date));
    }
    SECTION("adding a negative number")
    {
        CDate result = CDate(5, Month::JANUARY, 2000) + (-3);
        CHECK(result.GetDay() == 2);
        CHECK(result.GetMonth() == Month::JANUARY);
    }
    SECTION("going past maximum throws")
    {
        CDate date(31, Month::DECEMBER, 9999);
        CHECK_THROWS_AS(date + 1, std::out_of_range);
    }
}

TEST_CASE("Subtracting days (-)", "[arithmetic]")
{
    SECTION("01.01.2010 - 2 = 30.12.2009")
    {
        CDate result = CDate(1, Month::JANUARY, 2010) - 2;
        CHECK(result.GetDay() == 30);
        CHECK(result.GetMonth() == Month::DECEMBER);
        CHECK(result.GetYear() == 2009);
    }
    SECTION("going past minimum throws")
    {
        CHECK_THROWS_AS(CDate(1, Month::JANUARY, 1970) - 1, std::out_of_range);
    }
}

TEST_CASE("Difference between two dates", "[arithmetic]")
{
    const CDate d1(3, Month::JANUARY, 2010);
    const CDate d2(1, Month::JANUARY, 2010);
    CHECK((d1 - d2) == 2);
    CHECK((d2 - d1) == -2);
}

TEST_CASE("Operator +=", "[arithmetic]")
{
    CDate date(28, Month::FEBRUARY, 2000);
    date += 2;
    CHECK(date.GetDay() == 1);
    CHECK(date.GetMonth() == Month::MARCH);
    CHECK(date.GetYear() == 2000);
}

TEST_CASE("Operator -=", "[arithmetic]")
{
    CDate date(1, Month::MARCH, 2000);
    date -= 1;
    CHECK(date.GetDay() == 29);
    CHECK(date.GetMonth() == Month::FEBRUARY);
    CHECK(date.GetYear() == 2000);
}

TEST_CASE("Date comparison", "[comparison]")
{
    CDate earlier(1, Month::JANUARY, 2000);
    CDate later(2, Month::JANUARY, 2000);
    CDate same(1, Month::JANUARY, 2000);

    SECTION("==")
    {
        CHECK(earlier == same);
        CHECK_FALSE(earlier == later);
    }
    SECTION("!=")
    {
        CHECK(earlier != later);
        CHECK_FALSE(earlier != same);
    }
    SECTION("<")
    {
        CHECK(earlier < later);
        CHECK_FALSE(later < earlier);
        CHECK_FALSE(earlier < same);
    }
    SECTION(">")
    {
        CHECK(later > earlier);
        CHECK_FALSE(earlier > later);
    }
    SECTION("<=")
    {
        CHECK(earlier <= later);
        CHECK(earlier <= same);
        CHECK_FALSE(later <= earlier);
    }
    SECTION(">=")
    {
        CHECK(later >= earlier);
        CHECK(earlier >= same);
        CHECK_FALSE(earlier >= later);
    }
}


TEST_CASE("Output operator <<", "[io]")
{
    SECTION("DD.MM.YYYY format with leading zeros")
    {
        std::ostringstream os;
        os << CDate(5, Month::MARCH, 1970);
        CHECK(os.str() == "05.03.1970");
    }
    SECTION("01.01.1970")
    {
        std::ostringstream os;
        os << CDate(1, Month::JANUARY, 1970);
        CHECK(os.str() == "01.01.1970");
    }
    SECTION("31.12.9999")
    {
        std::ostringstream os;
        os << CDate(31, Month::DECEMBER, 9999);
        CHECK(os.str() == "31.12.9999");
    }
}

TEST_CASE("Input operator >>", "[io]")
{
    SECTION("valid date")
    {
        std::istringstream is("15.06.2000");
        CDate date;
        is >> date;
        REQUIRE(is);
        CHECK(date.GetDay() == 15);
        CHECK(date.GetMonth() == Month::JUNE);
        CHECK(date.GetYear() == 2000);
    }
    SECTION("invalid date sets failbit, date unchanged")
    {
        std::istringstream is("32.01.2000");
        CDate date(1, Month::JANUARY, 1970);
        is >> date;
        CHECK_FALSE(is);
        CHECK(date.GetDay() == 1);
        CHECK(date.GetMonth() == Month::JANUARY);
        CHECK(date.GetYear() == 1970);
    }
    SECTION("wrong format sets failbit")
    {
        std::istringstream is("2000/01/15");
        CDate date;
        is >> date;
        CHECK_FALSE(is);
    }
}
