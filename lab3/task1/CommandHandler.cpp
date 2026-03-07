#include "CommandHandler.h"
#include <iostream>

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
    it->second(args);
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
    if (!m_car.TurnOnEngine())
    {
        m_out << "Could not turn on the engine\n";
    }
}

void CommandHandler::EngineOff() const
{
    if (!m_car.TurnOffEngine())
    {
        m_out << "Сar must be stopped and in neutral gear\n";
    }
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

    if (!m_car.IsTurnedOn() && gear != 0)
    {
        m_out << "Cannot set gear while engine is off\n";
        return;
    }

    if (!m_car.SetGear(gear))
    {
        if (m_car.GetSpeed() != 0 && gear == -1)
        {
            m_out << "Cannot reverse while moving\n";
        }
        else
        {
            m_out << "Unsuitable current speed\n";
        }
    }
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

    if (!m_car.IsTurnedOn())
    {
        m_out << "Cannot set speed while engine is off\n";
        return;
    }

    if (m_car.GetGear() == 0 && speed > static_cast<int>(m_car.GetSpeed()))
    {
        m_out << "Cannot accelerate on neutral\n";
        return;
    }

    if (!m_car.SetSpeed(speed))
    {
        m_out << "Speed is out of gear range\n";
    }
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
