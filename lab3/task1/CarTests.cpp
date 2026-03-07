#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include <catch2/matchers/catch_matchers.hpp>

#include "Car.h"

// Начальное состояние
TEST_CASE("Car starts with engine off, neutral gear, zero speed and still direction")
{
    Car car;

    REQUIRE_FALSE(car.IsTurnedOn());
    REQUIRE(car.GetGear() == 0);
    REQUIRE(car.GetSpeed() == 0);
    REQUIRE(car.GetDirection() == Direction::Still);
}

// Двигатель
TEST_CASE("TurnOnEngine turns engine on")
{
    Car car;
    car.TurnOnEngine();

    REQUIRE(car.IsTurnedOn());
}

TEST_CASE("TurnOnEngine succeeds when engine is already on")
{
    Car car;
    car.TurnOnEngine();

    REQUIRE_NOTHROW(car.TurnOnEngine());
}

TEST_CASE("TurnOffEngine succeeds when engine is on, gear is neutral and speed is zero")
{
    Car car;
    car.TurnOnEngine();

    REQUIRE_NOTHROW(car.TurnOffEngine());
    REQUIRE_FALSE(car.IsTurnedOn());
}

TEST_CASE("TurnOffEngine succeeds when engine is already off")
{
    Car car;

    REQUIRE_NOTHROW(car.TurnOffEngine());
}

TEST_CASE("TurnOffEngine throws when speed is non-zero")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(10);

    REQUIRE_THROWS_AS(car.TurnOffEngine(), std::runtime_error);
    REQUIRE(car.IsTurnedOn());
}

TEST_CASE("TurnOffEngine throws when gear is not neutral")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);

    REQUIRE_THROWS_WITH(car.TurnOffEngine(), "Сar must be stopped and in neutral gear");
}

// Переключение передач - базовые случаи
TEST_CASE("SetGear to neutral is allowed with engine off")
{
    Car car;

    REQUIRE_NOTHROW(car.SetGear(0));
    REQUIRE(car.GetGear() == 0);
}

TEST_CASE("SetGear to non-neutral throws with engine off")
{
    Car car;

    REQUIRE_THROWS_WITH(car.SetGear(1), "Cannot set gear while engine is off");
    REQUIRE(car.GetGear() == 0);
}

TEST_CASE("SetGear to first gear succeeds at zero speed with engine on")
{
    Car car;
    car.TurnOnEngine();

    REQUIRE_NOTHROW(car.SetGear(1));
    REQUIRE(car.GetGear() == 1);
}

TEST_CASE("SetGear to same gear succeeds")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(15);

    REQUIRE_NOTHROW(car.SetGear(1));
}

// Задний ход
TEST_CASE("SetGear to reverse succeeds at zero speed")
{
    Car car;
    car.TurnOnEngine();

    REQUIRE_NOTHROW(car.SetGear(-1));
    REQUIRE(car.GetGear() == -1);
}

TEST_CASE("SetGear to reverse throws when moving forward")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(10);

    REQUIRE_THROWS_WITH(car.SetGear(-1), "Cannot reverse while moving");
}

TEST_CASE("SetGear to forward throws when moving backward on neutral")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    car.SetSpeed(10);
    car.SetGear(0);

    REQUIRE_THROWS_AS(car.SetGear(1), std::runtime_error);
}

TEST_CASE("SetGear to forward succeeds after stopping from reverse")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    car.SetSpeed(10);
    car.SetGear(0);
    car.SetSpeed(0);

    REQUIRE_NOTHROW(car.SetGear(1));
}

// Диапазоны передач при переключении
TEST_CASE("SetGear to second throws when speed is below its range")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(19);

    REQUIRE_THROWS_WITH(car.SetGear(2), "Unsuitable current speed");
}

TEST_CASE("SetGear to second succeeds when speed is within its range")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(20);

    REQUIRE_NOTHROW(car.SetGear(2));
}

TEST_CASE("SetGear to fifth succeeds when speed is within its range")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(30);
    car.SetGear(2);
    car.SetSpeed(50);
    car.SetGear(3);
    car.SetSpeed(60);
    car.SetGear(4);
    car.SetSpeed(90);
    car.SetGear(5);

    REQUIRE(car.GetGear() == 5);
}

// Установка скорости
TEST_CASE("SetSpeed throws with engine off")
{
    Car car;

    REQUIRE_THROWS_WITH(car.SetSpeed(10), "Cannot set speed while engine is off");
}

TEST_CASE("SetSpeed succeeds within gear range")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);

    REQUIRE_NOTHROW(car.SetSpeed(15));
    REQUIRE(car.GetSpeed() == 15);
}

TEST_CASE("SetSpeed throws above gear range")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);

    REQUIRE_THROWS_WITH(car.SetSpeed(31), "Speed is out of gear range");
    REQUIRE(car.GetSpeed() == 0);
}

TEST_CASE("SetSpeed throws below gear range")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(20);
    car.SetGear(2);

    REQUIRE_THROWS_WITH(car.SetSpeed(10), "Speed is out of gear range");
}

TEST_CASE("SetSpeed on neutral can decrease speed")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(30);
    car.SetGear(0);

    REQUIRE_NOTHROW(car.SetSpeed(10));
    REQUIRE(car.GetSpeed() == 10);
}

TEST_CASE("SetSpeed on neutral cannot increase speed")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(20);
    car.SetGear(0);

    REQUIRE_THROWS_WITH(car.SetSpeed(21), "Cannot accelerate on neutral");
    REQUIRE(car.GetSpeed() == 20);
}

TEST_CASE("SetSpeed on neutral to same value succeeds")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(20);
    car.SetGear(0);

    REQUIRE_NOTHROW(car.SetSpeed(20));
}

// Направление движения
TEST_CASE("Direction becomes forward when accelerating on forward gear")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(10);

    REQUIRE(car.GetDirection() == Direction::Forward);
}

TEST_CASE("Direction becomes backward when accelerating on reverse gear")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    car.SetSpeed(10);

    REQUIRE(car.GetDirection() == Direction::Backward);
}

TEST_CASE("Direction becomes still when speed reaches zero")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(10);
    car.SetSpeed(0);

    REQUIRE(car.GetDirection() == Direction::Still);
}

TEST_CASE("Direction remains backward while braking on neutral after reverse")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    car.SetSpeed(15);
    car.SetGear(0);
    car.SetSpeed(5);

    REQUIRE(car.GetDirection() == Direction::Backward);
}