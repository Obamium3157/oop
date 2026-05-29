#include <iostream>

#include "CMyString.h"

int main()
{
    CMyString str("Hello, world!");
    std::cout << str.SubString(4, -2) << std::endl;

    CMyString str1("Hello, world!");
    std::cout << str1.SubString(0, -1) << std::endl;
}
