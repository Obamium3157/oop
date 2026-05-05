#include <iostream>

#include "BodyCollection.h"
#include "ConsoleHandler.h"

int main()
{
    BodyCollection collection;
    ConsoleHandler handler(std::cin, std::cout);

    handler.FillCollection(collection);
    handler.PrintResults(collection);

    return 0;
}