#ifndef OOP_GEARBOX_H
#define OOP_GEARBOX_H
#include <optional>
#include <vector>

#include "Speed.h"

struct Gear
{
    const int number;
    const std::optional<SpeedRange> speedRange;
};

class Gearbox
{
public:
    Gearbox();
    [[nodiscard]] Gear GetCurrentGear() const;
    bool SwitchGear(int number, Speed currentCarSpeed, Direction currentDirection);
private:
    std::vector<Gear> m_gears;
    std::size_t m_currentGearIndex;
};


#endif //OOP_GEARBOX_H