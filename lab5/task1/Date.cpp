#include <iostream>

#include "CDate.h"

int main()
{
    CDate date;
    std::cin >> date;
    std::cout << "Date: " << date << std::endl;
    std::cout << "Date++: " << date++ << std::endl;
    std::cout << "++Date: " << ++date << std::endl;
    std::cout << "Date + 500: " << date + 500 << std::endl;
}
