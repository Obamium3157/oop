#ifndef OOP_SIMULATION_H
#define OOP_SIMULATION_H
#include <iosfwd>

#include "Car.h"

void Info(std::ostream& out, const Car& car);
void EngineOn(std::ostream& out, Car& car);
void EngineOff(std::ostream& out, Car& car);
void SetGear(std::ostream& out, Car& car, int gear);
void SetSpeed(std::ostream& out, Car& car, int speed);
void RunSimulation(std::istream& in, std::ostream& out, Car& car);

#endif //OOP_SIMULATION_H