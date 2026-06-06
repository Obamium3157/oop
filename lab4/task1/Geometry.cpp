#include <iostream>

#include "GeometryHandler.h"

int main()
{
    GeometryHandler handler;
    handler.ReadShapes(std::cin);
    handler.PrintResults(std::cout);
    return 0;
}
