#include <iostream>

#include "CDate.h"

int main()
{
    CDate date;
    std::cin >> date;
    std::cout << date << std::endl;
    std::cout << date++ << std::endl;
    std::cout << date + 500 << std::endl;
}
