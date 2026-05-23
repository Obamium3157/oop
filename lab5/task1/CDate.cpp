#include "CDate.h"
#include <iomanip>
#include <utility>

namespace
{
    bool IsLeapYear(unsigned year);
    unsigned DaysInMonth(Month month, unsigned year);
    unsigned CountLeapYearsUpTo(unsigned year);
    unsigned DaysToStartOfYear(unsigned year);
    unsigned ComputeTimestamp(unsigned day, Month month, unsigned year);
    bool IsDateValid(unsigned day, Month month, unsigned year);
    constexpr WeekDay WeekDayFromTimestamp(unsigned timestamp);
    unsigned YearsFromDays(unsigned days);
    unsigned YearFromTimestamp(unsigned timestamp);
    std::pair<Month, unsigned> ExtractMonthAndDay(unsigned timestamp);

    constexpr unsigned MinYear = 1970;
    constexpr unsigned MaxYear = 9999;
    const unsigned MaxValidTimestamp = ComputeTimestamp(31, Month::DECEMBER, MaxYear);
    constexpr unsigned GregorianCycleYears = 400;
    const unsigned GregorianCycleDays = GregorianCycleYears * 365 + CountLeapYearsUpTo(GregorianCycleYears);

    bool IsLeapYear(const unsigned year)
    {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    unsigned DaysInMonth(Month month, const unsigned year)
    {
        const unsigned daysPerMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == Month::FEBRUARY && IsLeapYear(year))
        {
            return 29;
        }
        return daysPerMonth[static_cast<unsigned>(month)];
    }

    unsigned CountLeapYearsUpTo(const unsigned year)
    {
        return year / 4 - year / 100 + year / 400;
    }

    unsigned DaysToStartOfYear(const unsigned year)
    {
        return (year - MinYear) * 365
            + CountLeapYearsUpTo(year - 1)
            - CountLeapYearsUpTo(MinYear - 1);
    }

    unsigned ComputeTimestamp(const unsigned day, Month month, const unsigned year)
    {
        auto timestamp = DaysToStartOfYear(year);
        const auto monthIndex = static_cast<unsigned>(month);
        for (unsigned i = 1; i < monthIndex; ++i)
        {
            timestamp += DaysInMonth(static_cast<Month>(i), year);
        }
        return timestamp + day - 1;
    }

    bool IsDateValid(const unsigned day, Month month, const unsigned year)
    {
        if (year < MinYear || year > MaxYear)
        {
            return false;
        }

        if (const auto monthIndex = static_cast<unsigned>(month); monthIndex < 1 || monthIndex > 12)
        {
            return false;
        }
        return day >= 1 && day <= DaysInMonth(month, year);
    }

    constexpr WeekDay WeekDayFromTimestamp(const unsigned timestamp)
    {
        constexpr unsigned daysInWeek = 7;
        constexpr unsigned epochWeekDayOffset = 4;
        return static_cast<WeekDay>((timestamp + epochWeekDayOffset) % daysInWeek);
    }

    unsigned YearsFromDays(const unsigned days)
    {
        return static_cast<unsigned>(
            static_cast<unsigned long long>(days) * GregorianCycleYears / GregorianCycleDays);
    }

    unsigned YearFromTimestamp(const unsigned timestamp)
    {
        unsigned year = MinYear + YearsFromDays(timestamp);

        if (DaysToStartOfYear(year + 1) <= timestamp)
        {
            ++year;
        }

        return year;
    }

    std::pair<Month, unsigned> ExtractMonthAndDay(const unsigned timestamp)
    {
        const unsigned year = YearFromTimestamp(timestamp);
        const unsigned remaining = timestamp - DaysToStartOfYear(year);
        const unsigned janFebDays = 31 + (IsLeapYear(year) ? 29 : 28);

        constexpr unsigned daysFromMarch1ToDec31 = 306;
        constexpr unsigned daysIn5Months = 153;

        const unsigned doy = (remaining < janFebDays)
                                 ? remaining + daysFromMarch1ToDec31
                                 : remaining - janFebDays;

        const unsigned mp = (5 * doy + 2) / daysIn5Months;
        const unsigned day = doy - (daysIn5Months * mp + 2) / 5 + 1;
        const unsigned month = (mp < 10) ? mp + 3 : mp - 9;

        return {static_cast<Month>(month), day};
    }
}

CDate::CDate(const unsigned day, const Month month, const unsigned year)
{
    if (!IsDateValid(day, month, year))
    {
        throw std::invalid_argument("Invalid date");
    }
    m_timestamp = ComputeTimestamp(day, month, year);
}

CDate::CDate(const unsigned timestamp)
{
    if (timestamp > MaxValidTimestamp)
    {
        throw std::invalid_argument("Timestamp out of valid range");
    }
    m_timestamp = timestamp;
}

CDate::CDate()
    : m_timestamp(0)
{
}

unsigned CDate::GetYear() const
{
    return YearFromTimestamp(m_timestamp);
}

Month CDate::GetMonth() const
{
    return ExtractMonthAndDay(m_timestamp).first;
}

unsigned CDate::GetDay() const
{
    return ExtractMonthAndDay(m_timestamp).second;
}

WeekDay CDate::GetWeekDay() const
{
    return WeekDayFromTimestamp(m_timestamp);
}

CDate& CDate::operator++()
{
    if (m_timestamp >= MaxValidTimestamp)
    {
        throw std::out_of_range("Date out of valid range");
    }

    ++m_timestamp;
    return *this;
}

CDate CDate::operator++(int)
{
    const CDate previous = *this;
    ++(*this);
    return previous;
}

CDate& CDate::operator--()
{
    if (m_timestamp == 0)
    {
        throw std::out_of_range("Date out of valid range");
    }

    --m_timestamp;
    return *this;
}

CDate CDate::operator--(int)
{
    const CDate previous = *this;
    --(*this);
    return previous;
}

CDate& CDate::operator+=(const int days)
{
    const long long result = static_cast<long long>(m_timestamp) + days;
    if (result < 0 || result > static_cast<long long>(MaxValidTimestamp))
    {
        throw std::out_of_range("Date out of valid range");
    }

    m_timestamp = static_cast<unsigned>(result);
    return *this;
}

CDate& CDate::operator-=(const int days)
{
    const long long result = static_cast<long long>(m_timestamp) - days;
    if (result < 0 || result > static_cast<long long>(MaxValidTimestamp))
    {
        throw std::out_of_range("Date out of valid range");
    }

    m_timestamp = static_cast<unsigned>(result);
    return *this;
}

CDate operator+(const CDate& date, const int days)
{
    CDate result = date;
    result += days;
    return result;
}

CDate operator+(const int days, const CDate& date)
{
    return date + days;
}

CDate operator-(const CDate& date, const int days)
{
    CDate result = date;
    result -= days;
    return result;
}

int operator-(const CDate& lhs, const CDate& rhs)
{
    return static_cast<int>(lhs.m_timestamp) - static_cast<int>(rhs.m_timestamp);
}

std::ostream& operator<<(std::ostream& os, const CDate& date)
{
    os << std::setfill('0')
        << std::setw(2) << date.GetDay() << '.'
        << std::setw(2) << static_cast<unsigned>(date.GetMonth()) << '.'
        << std::setw(4) << date.GetYear();
    return os;
}

std::istream& operator>>(std::istream& is, CDate& date)
{
    is >> std::ws;
    unsigned day, month, year;
    if (char dot1, dot2; !(is >> day >> dot1 >> month >> dot2 >> year) || dot1 != '.' || dot2 != '.')
    {
        is.setstate(std::ios::failbit);
        return is;
    }
    if (!IsDateValid(day, static_cast<Month>(month), year))
    {
        is.setstate(std::ios::failbit);
        return is;
    }
    date = CDate(day, static_cast<Month>(month), year);
    return is;
}
