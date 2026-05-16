#ifndef OOP_CDATE_H
#define OOP_CDATE_H

#include <compare>
#include <iosfwd>
#include <limits>
#include <utility>

enum class Month
{
    JANUARY = 1, FEBRUARY, MARCH, APRIL,
    MAY, JUNE, JULY, AUGUST, SEPTEMBER,
    OCTOBER, NOVEMBER, DECEMBER
};

enum class WeekDay
{
    SUNDAY = 0, MONDAY, TUESDAY, WEDNESDAY,
    THURSDAY, FRIDAY, SATURDAY
};

class CDate
{
public:
    CDate(unsigned day, Month month, unsigned year);
    explicit CDate(unsigned timestamp);
    CDate();

    unsigned GetDay() const;
    Month GetMonth() const;
    unsigned GetYear() const;
    WeekDay GetWeekDay() const;
    bool IsValid() const;

    CDate& operator++();
    CDate operator++(int);
    CDate& operator--();
    CDate operator--(int);

    CDate& operator+=(int days);
    CDate& operator-=(int days);

    auto operator<=>(const CDate& other) const = default;

    friend int operator-(const CDate& lhs, const CDate& rhs);
    friend std::ostream& operator<<(std::ostream& os, const CDate& date);
    friend std::istream& operator>>(std::istream& is, CDate& date);

private:
    static const unsigned MaxValidTimestamp;

    static constexpr unsigned InvalidTimestamp = std::numeric_limits<unsigned>::max();
    static constexpr unsigned MinYear = 1970;
    static constexpr unsigned MaxYear = 9999;
    static constexpr unsigned GregorianCycleYears = 400;
    static const unsigned GregorianCycleDays;

    unsigned m_timestamp;

    static bool IsLeapYear(unsigned year);
    static unsigned DaysInMonth(Month month, unsigned year);
    static unsigned CountLeapYearsUpTo(unsigned year);
    static unsigned DaysToStartOfYear(unsigned year);
    static unsigned ComputeTimestamp(unsigned day, Month month, unsigned year);
    static bool IsDateValid(unsigned day, Month month, unsigned year);
    static unsigned YearsFromDays(unsigned days);
    static constexpr WeekDay WeekDayFromTimestamp(unsigned timestamp);
    static unsigned YearFromTimestamp(unsigned timestamp);
    static std::pair<Month, unsigned> ExtractMonthAndDay(unsigned timestamp);
};


CDate operator+(const CDate& date, int days);
CDate operator+(int days, const CDate& date);
CDate operator-(const CDate& date, int days);


#endif //OOP_CDATE_H
