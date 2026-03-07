#ifndef OOP_SPEED_H
#define OOP_SPEED_H

using Speed = unsigned int;

struct SpeedRange
{
    const Speed minSpeed;
    const Speed maxSpeed;

    [[nodiscard]] bool Contains(const Speed speed) const
    {
        return speed >= minSpeed && speed <= maxSpeed;
    }
};

enum class Direction
{
    Backward = -1,
    Still = 0,
    Forward = 1,
};

#endif //OOP_SPEED_H