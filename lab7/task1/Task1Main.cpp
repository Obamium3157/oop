#include <list>
#include <windows.h>

#include "Athlete.h"
#include "FindMaxEx.h"

int main()
{
    SetConsoleOutputCP(65001);

    std::vector<Athlete> athletes =
    {
        {"Иванов Иван Иванович", 182.5, 78.0},
        {"Петров Петр Петрович", 195.0, 92.5},
        {"Сидоров Сидор Сидорович", 170.0, 110.3},
        {"Кузнецов Алексей Владимирович", 188.0, 85.0},
    };

    if (Athlete tallest{};
        FindMaxEx(athletes.begin(), athletes.end(), tallest, LessByHeight{}))
    {
        PrintAthlete("Самый высокий спортсмен:", tallest);
    }

    if (Athlete heaviest{};
        FindMaxEx(athletes.begin(), athletes.end(), heaviest, LessByWeight{}))
    {
        PrintAthlete("Самый тяжелый спортсмен:", heaviest);
    }


    std::list numbers{3, 1, 5, 6};
    int greatest;
    FindMaxEx(numbers.begin(), numbers.end(), greatest, [](const auto lhs, const auto rhs)
              noexcept
              {
                  return lhs < rhs;
              });
    std::cout << greatest << std::endl;

    return 0;
}
