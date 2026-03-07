#include "CommandHandler.h"

#include <iostream>
#include <stdexcept>

CommandHandler::CommandHandler(std::istream& in, std::ostream& out, Car& car)
    : m_in(in)
      , m_out(out)
      , m_car(car)
      , m_commands({
          {"Info", [this](std::istream&) { Info(); }},
          {"EngineOn", [this](std::istream&) { EngineOn(); }},
          {"EngineOff", [this](std::istream&) { EngineOff(); }},
          {"SetGear", [this](std::istream& args) { SetGear(args); }},
          {"SetSpeed", [this](std::istream& args) { SetSpeed(args); }},
          {"Help", [this](std::istream&) { Help(); }},
      })
{
}

void CommandHandler::Handle(const std::string& command, std::istream& args) const
{
    const auto it = m_commands.find(command);
    if (it == m_commands.end())
    {
        m_out << "Unknown command\n";
        return;
    }

    try
    {
        it->second(args);
    }
    catch (const std::runtime_error& e)
    {
        m_out << e.what() << "\n";
    }
}

void CommandHandler::Info() const
{
    m_out << "Engine: " << (m_car.IsTurnedOn() ? "on" : "off") << "\n";
    m_out << "Direction: ";
    switch (m_car.GetDirection())
    {
    case Direction::Forward:
        m_out << "forward";
        break;
    case Direction::Still:
        m_out << "standing still";
        break;
    case Direction::Backward:
        m_out << "backward";
        break;
    }
    m_out << "\n";
    m_out << "Speed: " << m_car.GetSpeed() << "\n";
    m_out << "Gear: " << m_car.GetGear() << "\n";
}

void CommandHandler::EngineOn() const
{
    m_car.TurnOnEngine();
}

void CommandHandler::EngineOff() const
{
    m_car.TurnOffEngine();
}

void CommandHandler::SetGear(std::istream& args) const
{
    int gear;
    if (!(args >> gear))
    {
        m_out << "Invalid command argument\n";
        return;
    }

    if (gear < -1 || gear > 5)
    {
        m_out << "Invalid gear\n";
        return;
    }

    m_car.SetGear(gear);
}

void CommandHandler::SetSpeed(std::istream& args) const
{
    int speed;
    if (!(args >> speed))
    {
        m_out << "Invalid command argument\n";
        return;
    }

    if (speed < 0)
    {
        m_out << "Speed cannot be negative\n";
        return;
    }

    m_car.SetSpeed(speed);
}

void CommandHandler::Help() const
{
    m_out << "Available commands:\n";
    m_out << "  Info\n";
    m_out << "  EngineOn\n";
    m_out << "  EngineOff\n";
    m_out << "  SetGear <-1..5>\n";
    m_out << "  SetSpeed <0..150>\n";
    m_out << "  Help\n";
}
