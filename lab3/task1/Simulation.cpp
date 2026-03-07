#include <iostream>
#include <string>
#include "Simulation.h"

void Info(std::ostream& out, const Car& car)
{
    out << "Engine: " << (car.IsTurnedOn() ? "on" : "off") << "\n";
    out << "Direction: ";
    switch (car.GetDirection())
    {
    case Direction::Forward:
        {
            out << "forward";
            break;
        }
    case Direction::Still:
        {
            out << "standing still";
            break;
        }
    case Direction::Backward:
        {
            out << "backward";
            break;
        }
    }
    out << "\n";
    out << "Speed: " << car.GetSpeed() << "\n";
    out << "Gear: " << car.GetGear() << "\n";
}

void EngineOn(std::ostream& out, Car& car)
{
    if (!car.TurnOnEngine())
    {
        out << "Could not turn on the engine\n";
    }
}

void EngineOff(std::ostream& out, Car& car)
{
    if (!car.TurnOffEngine())
    {
        out << "Сar must be stopped and in neutral gear\n";
    }
}

void SetGear(std::ostream& out, Car& car, const int gear)
{
    if (gear < -1 || gear > 5)
    {
        out << "Invalid gear\n";
        return;
    }

    if (!car.IsTurnedOn() && gear != 0)
    {
        out << "Cannot set gear while engine is off\n";
        return;
    }

    if (!car.SetGear(gear))
    {
        if (car.GetSpeed() != 0 && gear == -1)
        {
            out << "Cannot reverse while moving\n";
        }
        else
        {
            out << "Unsuitable current speed\n";
        }
    }
}

void SetSpeed(std::ostream& out, Car& car, const int speed)
{
    if (speed < 0)
    {
        out << "Speed cannot be negative\n";
        return;
    }

    if (!car.IsTurnedOn())
    {
        out << "Cannot set speed while engine is off\n";
        return;
    }

    if (car.GetGear() == 0 && speed > static_cast<int>(car.GetSpeed()))
    {
        out << "Cannot accelerate on neutral\n";
        return;
    }

    if (!car.SetSpeed(speed))
    {
        out << "Speed is out of gear range\n";
    }
}

void RunSimulation(std::istream& in, std::ostream& out, Car& car)
{
    std::string command;
    while (in >> command)
    {
        if (command == "Info")
        {
            Info(out, car);
        }
        else if (command == "EngineOn")
        {
            EngineOn(out, car);
        }
        else if (command == "EngineOff")
        {
            EngineOff(out, car);
        }
        else if (command == "SetGear")
        {
            if (int gear; in >> gear)
            {
                SetGear(out, car, gear);
            }
            else
            {
                out << "Invalid command argument\n";
            }
        }
        else if (command == "SetSpeed")
        {
            if (int speed; in >> speed)
            {
                SetSpeed(out, car, speed);
            }
            else
            {
                out << "Invalid command argument\n";
            }
        }
        else
        {
            out << "Unknown command\n";
        }
    }
}