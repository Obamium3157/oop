#include <iostream>

#include "CStringList.h"

int main()
{
    CStringList list;
    auto it = list.begin();
    std::cout << *it << std::endl;
}
