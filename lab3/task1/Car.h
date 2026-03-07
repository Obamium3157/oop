#ifndef OOP_CAR_H
#define OOP_CAR_H

#include "Gearbox.h"
#include "Speed.h"

class Car
{
public:
    Car();

    [[nodiscard]] bool IsTurnedOn() const;
    [[nodiscard]] Direction GetDirection() const;
    [[nodiscard]] Speed GetSpeed() const;
    [[nodiscard]] int GetGear() const;

    bool TurnOnEngine();
    bool TurnOffEngine();
    bool SetGear(int gear);
    bool SetSpeed(int speed);

private:
    Gearbox m_gearbox = Gearbox();
    Direction m_direction = Direction::Still;
    Speed m_speed = 0;
    bool m_isTurnedOn = false;
};


#endif //OOP_CAR_H
