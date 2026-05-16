#include "CDate.h"
#include <iomanip>

const unsigned CDate::MaxValidTimestamp = ComputeTimestamp(31, Month::DECEMBER, MaxYear);
const unsigned CDate::GregorianCycleDays = GregorianCycleYears * 365 + CountLeapYearsUpTo(GregorianCycleYears);

bool CDate::IsLeapYear(const unsigned year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

unsigned CDate::DaysInMonth(Month month, const unsigned year)
{
    const unsigned daysPerMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == Month::FEBRUARY && IsLeapYear(year))
    {
        return 29;
    }
    return daysPerMonth[static_cast<unsigned>(month)];
}

unsigned CDate::CountLeapYearsUpTo(const unsigned year)
{
    return year / 4 - year / 100 + year / 400;
}

unsigned CDate::DaysToStartOfYear(const unsigned year)
{
    return (year - MinYear) * 365
        + CountLeapYearsUpTo(year - 1)
        - CountLeapYearsUpTo(MinYear - 1);
}

unsigned CDate::ComputeTimestamp(const unsigned day, Month month, const unsigned year)
{
    auto timestamp = DaysToStartOfYear(year);
    const auto monthIndex = static_cast<unsigned>(month);
    for (unsigned i = 1; i < monthIndex; ++i)
    {
        timestamp += DaysInMonth(static_cast<Month>(i), year);
    }
    return timestamp + day - 1;
}

bool CDate::IsDateValid(const unsigned day, Month month, const unsigned year)
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

unsigned CDate::YearsFromDays(const unsigned days)
{
    return static_cast<unsigned>(
        static_cast<unsigned long long>(days) * GregorianCycleYears / GregorianCycleDays);
}

constexpr WeekDay CDate::WeekDayFromTimestamp(const unsigned timestamp)
{
    constexpr unsigned daysInWeek = 7;
    constexpr unsigned epochWeekDayOffset = 4;

    return static_cast<WeekDay>((timestamp + epochWeekDayOffset) % daysInWeek);
}

unsigned CDate::YearFromTimestamp(const unsigned timestamp)
{
    unsigned year = MinYear + YearsFromDays(timestamp);

    if (DaysToStartOfYear(year + 1) <= timestamp)
    {
        ++year;
    }

    return year;
}

std::pair<Month, unsigned> CDate::ExtractMonthAndDay(const unsigned timestamp)
{
    const unsigned year = YearFromTimestamp(timestamp);
    unsigned remaining = timestamp - DaysToStartOfYear(year);

    for (unsigned i = 1; i <= 12; ++i)
    {
        const unsigned daysInCurrentMonth = DaysInMonth(static_cast<Month>(i), year);
        if (remaining < daysInCurrentMonth)
        {
            return {static_cast<Month>(i), remaining + 1};
        }
        remaining -= daysInCurrentMonth;
    }
    return {Month::DECEMBER, 31};
}

CDate::CDate(const unsigned day, const Month month, const unsigned year)
    : m_timestamp(IsDateValid(day, month, year) ? ComputeTimestamp(day, month, year) : InvalidTimestamp)
{
}

CDate::CDate(const unsigned timestamp)
    : m_timestamp(timestamp <= MaxValidTimestamp ? timestamp : InvalidTimestamp)
{
}

CDate::CDate()
    : m_timestamp(0)
{
}

bool CDate::IsValid() const
{
    return m_timestamp != InvalidTimestamp;
}

unsigned CDate::GetYear() const
{
    if (!IsValid())
    {
        return 0;
    }
    return YearFromTimestamp(m_timestamp);
}

Month CDate::GetMonth() const
{
    if (!IsValid())
    {
        return Month::JANUARY;
    }
    return ExtractMonthAndDay(m_timestamp).first;
}

unsigned CDate::GetDay() const
{
    if (!IsValid())
    {
        return 0;
    }
    return ExtractMonthAndDay(m_timestamp).second;
}

WeekDay CDate::GetWeekDay() const
{
    if (!IsValid())
    {
        return WeekDay::SUNDAY;
    }
    return WeekDayFromTimestamp(m_timestamp);
}

CDate& CDate::operator++()
{
    if (IsValid())
    {
        if (m_timestamp >= MaxValidTimestamp)
        {
            m_timestamp = InvalidTimestamp;
        }
        else
        {
            ++m_timestamp;
        }
    }

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
    if (IsValid())
    {
        if (m_timestamp == 0)
        {
            m_timestamp = InvalidTimestamp;
        }
        else
        {
            --m_timestamp;
        }
    }

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
    if (!IsValid())
    {
        return *this;
    }

    if (const long long result = static_cast<long long>(m_timestamp) + days;
        result < 0 || result > static_cast<long long>(MaxValidTimestamp))
    {
        m_timestamp = InvalidTimestamp;
    }
    else
    {
        m_timestamp = static_cast<unsigned>(result);
    }

    return *this;
}

CDate& CDate::operator-=(const int days)
{
    if (!IsValid())
    {
        return *this;
    }

    if (const long long result = static_cast<long long>(m_timestamp) - days;
        result < 0 || result > static_cast<long long>(MaxValidTimestamp))
    {
        m_timestamp = InvalidTimestamp;
    }
    else
    {
        m_timestamp = static_cast<unsigned>(result);
    }

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
    if (!lhs.IsValid() || !rhs.IsValid())
    {
        return 0;
    }

    return static_cast<int>(lhs.m_timestamp) - static_cast<int>(rhs.m_timestamp);
}

std::ostream& operator<<(std::ostream& os, const CDate& date)
{
    if (!date.IsValid())
    {
        os << "INVALID";
        return os;
    }
    os << std::setfill('0')
        << std::setw(2) << date.GetDay() << '.'
        << std::setw(2) << static_cast<unsigned>(date.GetMonth()) << '.'
        << std::setw(4) << date.GetYear();
    return os;
}

std::istream& operator>>(std::istream& is, CDate& date)
{
    is >> std::ws;
    if (is.peek() == 'I')
    {
        std::string token;
        is >> token;
        if (token == "INVALID")
        {
            date = CDate(CDate::InvalidTimestamp);
        }
        else
        {
            is.setstate(std::ios::failbit);
        }
        return is;
    }

    unsigned day, month, year;

    if (char dot1, dot2; !(is >> day >> dot1 >> month >> dot2 >> year) || dot1 != '.' || dot2 != '.')
    {
        is.setstate(std::ios::failbit);
        return is;
    }

    date = CDate(day, static_cast<Month>(month), year);
    return is;
}
