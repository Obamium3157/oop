#include "Car.h"

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

bool Car::TurnOnEngine()
{
    m_isTurnedOn = true;

    return true;
}

bool Car::TurnOffEngine()
{
    if (!m_isTurnedOn)
    {
        return true;
    }

    const bool isNeutral = m_gearbox.GetCurrentGear().number == 0;

    if (!isNeutral || m_direction != Direction::Still)
    {
        return false;
    }

    m_isTurnedOn = false;
    return true;
}


bool Car::SetGear(const int gear)
{
    if (!m_isTurnedOn && gear != 0)
    {
        return false;
    }

    return m_gearbox.SwitchGear(gear, m_speed, m_direction);
}

bool Car::SetSpeed(const int speed)
{
    if (speed < 0)
    {
        return false;
    }

    if (!m_isTurnedOn)
    {
        return false;
    }

    const auto unsignedSpeed = static_cast<Speed>(speed);
    const Gear currentGear = m_gearbox.GetCurrentGear();
    const bool isNeutral = currentGear.number == 0;

    if (isNeutral && unsignedSpeed > m_speed)
    {
        return false;
    }

    if (currentGear.speedRange.has_value() && !currentGear.speedRange->Contains(unsignedSpeed))
    {
        return false;
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

    return true;
}
