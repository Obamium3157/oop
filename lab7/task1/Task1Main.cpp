#include <windows.h>

#include "Athlete.h"
#include "FindMaxEx.h"

int main()
{
    SetConsoleOutputCP(65001);

    std::vector<Athlete> const athletes =
    {
        {"Иванов Иван Иванович", 182.5, 78.0},
        {"Петров Петр Петрович", 195.0, 92.5},
        {"Сидоров Сидор Сидорович", 170.0, 110.3},
        {"Кузнецов Алексей Владимирович", 188.0, 85.0},
    };

    Athlete tallest{};
    Athlete heaviest{};

    if (FindMaxEx(athletes, tallest, LessByHeight{}))
    {
        PrintAthlete("Самый высокий спортсмен:", tallest);
    }

    if (FindMaxEx(athletes, heaviest, LessByWeight{}))
    {
        PrintAthlete("Самый тяжелый спортсмен:", heaviest);
    }

    return 0;
}
