#ifndef OOP_CDATE_H
#define OOP_CDATE_H

#include <compare>
#include <iosfwd>

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
    unsigned m_timestamp;
};


CDate operator+(const CDate& date, int days);
CDate operator+(int days, const CDate& date);
CDate operator-(const CDate& date, int days);


#endif //OOP_CDATE_H
