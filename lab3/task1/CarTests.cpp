#include <catch2/catch_test_macros.hpp>
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
TEST_CASE("TurnOnEngine returns true and turns engine on")
{
    Car car;

    REQUIRE(car.TurnOnEngine());
    REQUIRE(car.IsTurnedOn());
}

TEST_CASE("TurnOnEngine returns true when engine is already on")
{
    Car car;
    car.TurnOnEngine();

    REQUIRE(car.TurnOnEngine());
}

TEST_CASE("TurnOffEngine succeeds when engine is on, gear is neutral and speed is zero")
{
    Car car;
    car.TurnOnEngine();

    REQUIRE(car.TurnOffEngine());
    REQUIRE_FALSE(car.IsTurnedOn());
}

TEST_CASE("TurnOffEngine returns true when engine is already off")
{
    Car car;

    REQUIRE(car.TurnOffEngine());
}

TEST_CASE("TurnOffEngine fails when speed is non-zero")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(10);

    REQUIRE_FALSE(car.TurnOffEngine());
    REQUIRE(car.IsTurnedOn());
}

TEST_CASE("TurnOffEngine fails when gear is not neutral")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);

    REQUIRE_FALSE(car.TurnOffEngine());
}

// Переключение передач - базовые случаи
TEST_CASE("SetGear to neutral is allowed with engine off")
{
    Car car;

    REQUIRE(car.SetGear(0));
    REQUIRE(car.GetGear() == 0);
}

TEST_CASE("SetGear to non-neutral fails with engine off")
{
    Car car;

    REQUIRE_FALSE(car.SetGear(1));
    REQUIRE(car.GetGear() == 0);
}

TEST_CASE("SetGear to first gear succeeds at zero speed with engine on")
{
    Car car;
    car.TurnOnEngine();

    REQUIRE(car.SetGear(1));
    REQUIRE(car.GetGear() == 1);
}

TEST_CASE("SetGear fails for out-of-range gear number")
{
    Car car;
    car.TurnOnEngine();

    REQUIRE_FALSE(car.SetGear(6));
    REQUIRE_FALSE(car.SetGear(-2));
}

TEST_CASE("SetGear to same gear succeeds")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(15);

    REQUIRE(car.SetGear(1));
}

// Задний ход
TEST_CASE("SetGear to reverse succeeds at zero speed")
{
    Car car;
    car.TurnOnEngine();

    REQUIRE(car.SetGear(-1));
    REQUIRE(car.GetGear() == -1);
}

TEST_CASE("SetGear to reverse fails when moving forward")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(10);

    REQUIRE_FALSE(car.SetGear(-1));
}

TEST_CASE("SetGear to forward fails when moving backward on neutral")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    car.SetSpeed(10);
    car.SetGear(0);

    REQUIRE_FALSE(car.SetGear(1));
}

TEST_CASE("SetGear to forward succeeds after stopping from reverse")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    car.SetSpeed(10);
    car.SetGear(0);
    car.SetSpeed(0);

    REQUIRE(car.SetGear(1));
}

// Диапазоны передач при переключении
TEST_CASE("SetGear to second fails when speed is below its range")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(19);

    REQUIRE_FALSE(car.SetGear(2));
}

TEST_CASE("SetGear to second succeeds when speed is within its range")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(20);

    REQUIRE(car.SetGear(2));
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
TEST_CASE("SetSpeed fails with engine off")
{
    Car car;

    REQUIRE_FALSE(car.SetSpeed(10));
}

TEST_CASE("SetSpeed fails for negative value")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);

    REQUIRE_FALSE(car.SetSpeed(-1));
}

TEST_CASE("SetSpeed succeeds within gear range")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);

    REQUIRE(car.SetSpeed(15));
    REQUIRE(car.GetSpeed() == 15);
}

TEST_CASE("SetSpeed fails above gear range")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);

    REQUIRE_FALSE(car.SetSpeed(31));
    REQUIRE(car.GetSpeed() == 0);
}

TEST_CASE("SetSpeed fails below gear range")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(20);
    car.SetGear(2);

    REQUIRE_FALSE(car.SetSpeed(10));
}

TEST_CASE("SetSpeed on neutral can decrease speed")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(30);
    car.SetGear(0);

    REQUIRE(car.SetSpeed(10));
    REQUIRE(car.GetSpeed() == 10);
}

TEST_CASE("SetSpeed on neutral cannot increase speed")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(20);
    car.SetGear(0);

    REQUIRE_FALSE(car.SetSpeed(21));
    REQUIRE(car.GetSpeed() == 20);
}

TEST_CASE("SetSpeed on neutral to same value succeeds")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(20);
    car.SetGear(0);

    REQUIRE(car.SetSpeed(20));
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