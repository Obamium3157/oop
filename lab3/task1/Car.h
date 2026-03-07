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
    bool m_isTurnedOn;
    Direction m_direction;
    Speed m_speed;
    Gearbox m_gearbox;
};


#endif //OOP_CAR_H