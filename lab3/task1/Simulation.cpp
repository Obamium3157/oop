#include <iostream>
#include <string>
#include "Simulation.h"

#include <sstream>

#include "CommandHandler.h"

void RunSimulation(std::istream& in, std::ostream& out, Car& car)
{
    const CommandHandler handler(in, out, car);

    std::string line;
    while (std::getline(in, line))
    {
        std::istringstream lineStream(line);
        if (std::string command; lineStream >> command)
        {
            handler.Handle(command, lineStream);
        }
    }
}
