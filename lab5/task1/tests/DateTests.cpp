#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "../CDate.h"

TEST_CASE("Default constructor creates 01.01.1970", "[constructor]")
{
    const CDate date;
    REQUIRE(date.IsValid());
    CHECK(date.GetDay() == 1);
    CHECK(date.GetMonth() == Month::JANUARY);
    CHECK(date.GetYear() == 1970);
}

TEST_CASE("Timestamp constructor", "[constructor]")
{
    SECTION("timestamp 0 = 01.01.1970")
    {
        CDate date(0u);
        REQUIRE(date.IsValid());
        CHECK(date.GetDay() == 1);
        CHECK(date.GetMonth() == Month::JANUARY);
        CHECK(date.GetYear() == 1970);
    }
    SECTION("timestamp 2 = 03.01.1970")
    {
        CDate date(2u);
        REQUIRE(date.IsValid());
        CHECK(date.GetDay() == 3);
        CHECK(date.GetMonth() == Month::JANUARY);
        CHECK(date.GetYear() == 1970);
    }
    SECTION("timestamp 32 = 02.02.1970")
    {
        CDate date(32u);
        REQUIRE(date.IsValid());
        CHECK(date.GetDay() == 2);
        CHECK(date.GetMonth() == Month::FEBRUARY);
        CHECK(date.GetYear() == 1970);
    }
}

TEST_CASE("Day/month/year constructor", "[constructor]")
{
    SECTION("valid date")
    {
        CDate date(15, Month::JUNE, 2000);
        REQUIRE(date.IsValid());
        CHECK(date.GetDay() == 15);
        CHECK(date.GetMonth() == Month::JUNE);
        CHECK(date.GetYear() == 2000);
    }
    SECTION("maximum valid date: 31.12.9999")
    {
        CDate date(31, Month::DECEMBER, 9999);
        REQUIRE(date.IsValid());
        CHECK(date.GetDay() == 31);
        CHECK(date.GetMonth() == Month::DECEMBER);
        CHECK(date.GetYear() == 9999);
    }
    SECTION("invalid day") { CHECK_FALSE(CDate(32, Month::JANUARY, 2000).IsValid()); }
    SECTION("zero day") { CHECK_FALSE(CDate(0, Month::JANUARY, 2000).IsValid()); }
    SECTION("invalid month") { CHECK_FALSE(CDate(1, static_cast<Month>(13), 2000).IsValid()); }
    SECTION("year before 1970") { CHECK_FALSE(CDate(1, Month::JANUARY, 1969).IsValid()); }
    SECTION("year after 9999") { CHECK_FALSE(CDate(1, Month::JANUARY, 10000).IsValid()); }
}


TEST_CASE("Leap years", "[leap]")
{
    SECTION("2000 is a leap year: February 29 is valid")
    {
        CDate date(29, Month::FEBRUARY, 2000);
        REQUIRE(date.IsValid());
        CHECK(date.GetDay() == 29);
    }
    SECTION("1900 is not a leap year: February 29 is invalid")
    {
        CHECK_FALSE(CDate(29, Month::FEBRUARY, 1900).IsValid());
    }
    SECTION("1972 is a leap year")
    {
        CDate date(29, Month::FEBRUARY, 1972);
        REQUIRE(date.IsValid());
        CHECK(date.GetDay() == 29);
        CHECK(date.GetMonth() == Month::FEBRUARY);
        CHECK(date.GetYear() == 1972);
    }
    SECTION("2001 is not a leap year: February 29 is invalid")
    {
        CHECK_FALSE(CDate(29, Month::FEBRUARY, 2001).IsValid());
    }
}


TEST_CASE("Day of week", "[weekday]")
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
    SECTION("01.01.2024 is Monday (known date)")
    {
        CHECK(CDate(1, Month::JANUARY, 2024).GetWeekDay() == WeekDay::MONDAY);
    }
    SECTION("01.01.2000 is Saturday")
    {
        CHECK(CDate(1, Month::JANUARY, 2000).GetWeekDay() == WeekDay::SATURDAY);
    }
}

TEST_CASE("Prefix ++", "[increment]")
{
    SECTION("regular advance to the next day")
    {
        CDate date(1, Month::JANUARY, 1970);
        CDate& ref = ++date;
        CHECK(&ref == &date);
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
    SECTION("crossing end of February in a leap year")
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
    SECTION("going past maximum => invalid")
    {
        CDate date(31, Month::DECEMBER, 9999);
        ++date;
        CHECK_FALSE(date.IsValid());
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
    SECTION("regular move to the previous day")
    {
        CDate date(2, Month::JANUARY, 1970);
        CDate& ref = --date;
        CHECK(&ref == &date);
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
    SECTION("crossing start of March in a leap year")
    {
        CDate date(1, Month::MARCH, 2000);
        --date;
        CHECK(date.GetDay() == 29);
        CHECK(date.GetMonth() == Month::FEBRUARY);
    }
    SECTION("decrement from epoch => invalid")
    {
        CDate date(1, Month::JANUARY, 1970);
        --date;
        CHECK_FALSE(date.IsValid());
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

TEST_CASE("Operations on invalid date do not change it", "[invalid]")
{
    CDate date(1, Month::JANUARY, 1970);
    --date;
    REQUIRE_FALSE(date.IsValid());

    ++date;
    CHECK_FALSE(date.IsValid());
    --date;
    CHECK_FALSE(date.IsValid());
    date += 100;
    CHECK_FALSE(date.IsValid());
    date -= 100;
    CHECK_FALSE(date.IsValid());
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
    SECTION("going past maximum => invalid")
    {
        CDate date(31, Month::DECEMBER, 9999);
        CHECK_FALSE((date + 1).IsValid());
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
    SECTION("going past minimum => invalid")
    {
        CHECK_FALSE((CDate(1, Month::JANUARY, 1970) - 1).IsValid());
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
    SECTION("invalid date => INVALID")
    {
        std::ostringstream os;
        os << CDate(32, Month::JANUARY, 2000);
        CHECK(os.str() == "INVALID");
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
    SECTION("keyword INVALID")
    {
        std::istringstream is("INVALID");
        CDate date(1, Month::JANUARY, 2000);
        is >> date;
        REQUIRE(is);
        CHECK_FALSE(date.IsValid());
    }
    SECTION("invalid date (day 32) => IsValid() == false")
    {
        std::istringstream is("32.01.2000");
        CDate date;
        is >> date;
        CHECK_FALSE(date.IsValid());
    }
}