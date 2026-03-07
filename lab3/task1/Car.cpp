#include "Car.h"

#include <stdexcept>

Car::Car() = default;

bool Car::IsTurnedOn() const
{
    return m_isTurnedOn;
}

Direction Car::GetDirection() const
{
    return m_direction;
}

Speed Car::GetSpeed() const
{
    return m_speed;
}

int Car::GetGear() const
{
    return m_gearbox.GetCurrentGear().number;
}

void Car::TurnOnEngine()
{
    m_isTurnedOn = true;
}

void Car::TurnOffEngine()
{
    if (!m_isTurnedOn)
    {
        throw std::runtime_error("Engine must be turned on");
    }

    const bool isNeutral = m_gearbox.GetCurrentGear().number == 0;

    if (!isNeutral || m_direction != Direction::Still)
    {
        throw std::runtime_error("Сar must be stopped and in neutral gear");
    }

    m_isTurnedOn = false;
}


void Car::SetGear(const int gear)
{
    if (!m_isTurnedOn && gear != 0)
    {
        throw std::runtime_error("Cannot set gear while engine is off");
    }

    if (gear == -1 && m_speed != 0)
    {
        throw std::runtime_error("Cannot reverse while moving");
    }

    if (!m_gearbox.SwitchGear(gear, m_speed, m_direction))
    {
        throw std::runtime_error("Unsuitable current speed");
    }
}

void Car::SetSpeed(const int speed)
{
    if (speed < 0)
    {
        throw std::runtime_error("Speed cannot be negative");
    }

    if (!m_isTurnedOn)
    {
        throw std::runtime_error("Cannot set speed while engine is off");
    }

    const auto unsignedSpeed = static_cast<Speed>(speed);
    const Gear currentGear = m_gearbox.GetCurrentGear();
    const bool isNeutral = currentGear.number == 0;

    if (isNeutral && unsignedSpeed > m_speed)
    {
        throw std::runtime_error("Cannot accelerate on neutral");
    }

    if (currentGear.speedRange.has_value() && !currentGear.speedRange->Contains(unsignedSpeed))
    {
        throw std::runtime_error("Speed is out of gear range");
    }

    m_speed = unsignedSpeed;

    if (m_speed == 0)
    {
        m_direction = Direction::Still;
    }
    else if (m_direction == Direction::Still)
    {
        m_direction = currentGear.number == -1 ? Direction::Backward : Direction::Forward;
    }
}
