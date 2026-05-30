#include <iostream>

#include "CMyArray.h"

int main()
{
    CMyArray<int> arr;
    arr.PushBack(1);
    arr.PushBack(2);
    arr.PushBack(3);

    for (const auto el : arr)
    {
        std::cout << el;
    }
}
