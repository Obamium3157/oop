#include "Gearbox.h"

#include <algorithm>

Gearbox::Gearbox()
    : m_gears({
          {-1, SpeedRange{0, 20}},
          {0, std::nullopt},
          {1, SpeedRange{0, 30}},
          {2, SpeedRange{20, 50}},
          {3, SpeedRange{30, 60}},
          {4, SpeedRange{40, 90}},
          {5, SpeedRange{50, 150}},
          {6, SpeedRange{130, 1500}},
      })
    , m_currentGearIndex(1)
{
}

Gear Gearbox::GetCurrentGear() const
{
    return m_gears[m_currentGearIndex];
}

bool Gearbox::SwitchGear(const int number, const Speed currentCarSpeed, const Direction currentDirection)
{
    const auto it = std::find_if(m_gears.begin(), m_gears.end(),
        [&](const Gear& gear) {
            return gear.number == number;
        });

    if (it == m_gears.end())
    {
        return false;
    }

    const Gear& newGear = *it;

    if (newGear.speedRange.has_value() && !newGear.speedRange->Contains(currentCarSpeed))
    {
        return false;
    }

    const bool isSwitchingToReverse = newGear.number == -1;
    const bool isSwitchingToForward = newGear.number > 0;
    const bool isMovingBackward = currentDirection == Direction::Backward;

    if (isSwitchingToReverse && currentCarSpeed != 0)
    {
        return false;
    }

    if (isSwitchingToForward && isMovingBackward && currentCarSpeed != 0)
    {
        return false;
    }

    m_currentGearIndex = std::distance(m_gears.begin(), it);
    return true;
}