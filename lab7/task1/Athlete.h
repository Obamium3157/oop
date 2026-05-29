#ifndef OOP_ATHLETE_H
#define OOP_ATHLETE_H

#include <iostream>
#include <string>

struct Athlete
{
    std::string fullName;
    double heightCm;
    double weightKg;
};

struct LessByHeight
{
    bool operator()(const Athlete& left, const Athlete& right) const noexcept
    {
        return left.heightCm < right.heightCm;
    }
};

struct LessByWeight
{
    bool operator()(const Athlete& left, const Athlete& right) const noexcept
    {
        return left.weightKg < right.weightKg;
    }
};

inline void PrintAthlete(const std::string& label, const Athlete& athlete)
{
    std::cout << label << "\n"
        << "  ФИО:  " << athlete.fullName << "\n"
        << "  Рост: " << athlete.heightCm << " см\n"
        << "  Вес:  " << athlete.weightKg << " кг\n\n";
}

#endif //OOP_ATHLETE_H
