#include <iostream>
#include <sstream>

#include "Calculator.h"
#include "CommandParser.h"

// Выучить разницу между композицией и агрегацией
int main()
{
    Calculator calculator;
    const CommandParser parser{std::cin, std::cout, calculator};

    std::string line;
    while (std::getline(std::cin, line))
    {
        std::istringstream stream(line);
        std::string command;
        if (!(stream >> command))
        {
            continue;
        }
        parser.Parse(command, stream);
    }
}
