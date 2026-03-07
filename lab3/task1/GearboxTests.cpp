#include <catch2/catch_test_macros.hpp>
#include "Gearbox.h"

// Начальное состояние
TEST_CASE("Gearbox starts in neutral gear")
{
    Gearbox gearbox;

    REQUIRE(gearbox.GetCurrentGear().number == 0);
}

TEST_CASE("Gearbox neutral gear has no speed range")
{
    Gearbox gearbox;

    REQUIRE_FALSE(gearbox.GetCurrentGear().speedRange.has_value());
}

// Несуществующая передача
TEST_CASE("SwitchGear fails for gear above 5")
{
    Gearbox gearbox;

    REQUIRE_FALSE(gearbox.SwitchGear(6, 0, Direction::Still));
    REQUIRE(gearbox.GetCurrentGear().number == 0);
}

TEST_CASE("SwitchGear fails for gear below -1")
{
    Gearbox gearbox;

    REQUIRE_FALSE(gearbox.SwitchGear(-2, 0, Direction::Still));
    REQUIRE(gearbox.GetCurrentGear().number == 0);
}

// Переключение на нейтраль
TEST_CASE("SwitchGear to neutral succeeds at any speed when standing still")
{
    Gearbox gearbox;

    REQUIRE(gearbox.SwitchGear(0, 0, Direction::Still));
    REQUIRE(gearbox.GetCurrentGear().number == 0);
}

TEST_CASE("SwitchGear to neutral succeeds while moving forward")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);

    REQUIRE(gearbox.SwitchGear(0, 15, Direction::Forward));
    REQUIRE(gearbox.GetCurrentGear().number == 0);
}

TEST_CASE("SwitchGear to neutral succeeds while moving backward")
{
    Gearbox gearbox;
    gearbox.SwitchGear(-1, 0, Direction::Still);

    REQUIRE(gearbox.SwitchGear(0, 10, Direction::Backward));
    REQUIRE(gearbox.GetCurrentGear().number == 0);
}

// Переключение на задний ход
TEST_CASE("SwitchGear to reverse succeeds at zero speed when standing still")
{
    Gearbox gearbox;

    REQUIRE(gearbox.SwitchGear(-1, 0, Direction::Still));
    REQUIRE(gearbox.GetCurrentGear().number == -1);
}

TEST_CASE("SwitchGear to reverse fails when moving forward")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);

    REQUIRE_FALSE(gearbox.SwitchGear(-1, 10, Direction::Forward));
    REQUIRE(gearbox.GetCurrentGear().number == 1);
}

TEST_CASE("SwitchGear to reverse fails when moving backward on neutral")
{
    Gearbox gearbox;
    gearbox.SwitchGear(-1, 0, Direction::Still);
    gearbox.SwitchGear(0, 10, Direction::Backward);

    REQUIRE_FALSE(gearbox.SwitchGear(-1, 10, Direction::Backward));
    REQUIRE(gearbox.GetCurrentGear().number == 0);
}

TEST_CASE("SwitchGear to reverse succeeds at speed within its range when standing still")
{
    Gearbox gearbox;

    REQUIRE(gearbox.SwitchGear(-1, 0, Direction::Still));
}

TEST_CASE("SwitchGear to reverse fails above its max speed")
{
    Gearbox gearbox;

    REQUIRE_FALSE(gearbox.SwitchGear(-1, 21, Direction::Forward));
}

// Переключение с заднего хода на переднюю передачу
TEST_CASE("SwitchGear to first fails when moving backward on neutral")
{
    Gearbox gearbox;
    gearbox.SwitchGear(-1, 0, Direction::Still);
    gearbox.SwitchGear(0, 10, Direction::Backward);

    REQUIRE_FALSE(gearbox.SwitchGear(1, 10, Direction::Backward));
}

TEST_CASE("SwitchGear to first succeeds after full stop from backward")
{
    Gearbox gearbox;
    gearbox.SwitchGear(-1, 0, Direction::Still);
    gearbox.SwitchGear(0, 10, Direction::Backward);

    REQUIRE(gearbox.SwitchGear(1, 0, Direction::Still));
    REQUIRE(gearbox.GetCurrentGear().number == 1);
}

// Диапазоны скоростей передач
TEST_CASE("SwitchGear to first succeeds at min speed of its range")
{
    Gearbox gearbox;

    REQUIRE(gearbox.SwitchGear(1, 0, Direction::Still));
}

TEST_CASE("SwitchGear to first succeeds at max speed of its range")
{
    Gearbox gearbox;

    REQUIRE(gearbox.SwitchGear(1, 30, Direction::Still));
}

TEST_CASE("SwitchGear to first fails above its max speed")
{
    Gearbox gearbox;

    REQUIRE_FALSE(gearbox.SwitchGear(1, 31, Direction::Forward));
}

TEST_CASE("SwitchGear to second succeeds at min speed of its range")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);

    REQUIRE(gearbox.SwitchGear(2, 20, Direction::Forward));
}

TEST_CASE("SwitchGear to second fails below its min speed")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);

    REQUIRE_FALSE(gearbox.SwitchGear(2, 19, Direction::Forward));
    REQUIRE(gearbox.GetCurrentGear().number == 1);
}

TEST_CASE("SwitchGear to second succeeds at max speed of its range")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);

    REQUIRE(gearbox.SwitchGear(2, 50, Direction::Forward));
}

TEST_CASE("SwitchGear to second fails above its max speed")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);

    REQUIRE_FALSE(gearbox.SwitchGear(2, 51, Direction::Forward));
}

TEST_CASE("SwitchGear to third succeeds within its range")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);
    gearbox.SwitchGear(2, 30, Direction::Forward);

    REQUIRE(gearbox.SwitchGear(3, 30, Direction::Forward));
}

TEST_CASE("SwitchGear to fourth succeeds within its range")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);
    gearbox.SwitchGear(2, 40, Direction::Forward);
    gearbox.SwitchGear(3, 40, Direction::Forward);

    REQUIRE(gearbox.SwitchGear(4, 40, Direction::Forward));
}

TEST_CASE("SwitchGear to fifth succeeds at min speed of its range")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);
    gearbox.SwitchGear(2, 50, Direction::Forward);
    gearbox.SwitchGear(3, 50, Direction::Forward);
    gearbox.SwitchGear(4, 50, Direction::Forward);

    REQUIRE(gearbox.SwitchGear(5, 50, Direction::Forward));
}

TEST_CASE("SwitchGear to fifth succeeds at max speed of its range")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);
    gearbox.SwitchGear(2, 50, Direction::Forward);
    gearbox.SwitchGear(3, 50, Direction::Forward);
    gearbox.SwitchGear(4, 50, Direction::Forward);

    REQUIRE(gearbox.SwitchGear(5, 150, Direction::Forward));
}

TEST_CASE("SwitchGear to fifth fails above its max speed")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);
    gearbox.SwitchGear(2, 50, Direction::Forward);
    gearbox.SwitchGear(3, 50, Direction::Forward);
    gearbox.SwitchGear(4, 50, Direction::Forward);

    REQUIRE_FALSE(gearbox.SwitchGear(5, 151, Direction::Forward));
}

// Переключение на ту же передачу
TEST_CASE("SwitchGear to same forward gear succeeds")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);

    REQUIRE(gearbox.SwitchGear(1, 15, Direction::Forward));
    REQUIRE(gearbox.GetCurrentGear().number == 1);
}

TEST_CASE("SwitchGear to same neutral gear succeeds")
{
    Gearbox gearbox;

    REQUIRE(gearbox.SwitchGear(0, 0, Direction::Still));
    REQUIRE(gearbox.GetCurrentGear().number == 0);
}

TEST_CASE("SwitchGear to same reverse gear succeeds")
{
    Gearbox gearbox;
    gearbox.SwitchGear(-1, 0, Direction::Still);

    REQUIRE(gearbox.SwitchGear(-1, 0, Direction::Still));
    REQUIRE(gearbox.GetCurrentGear().number == -1);
}

// Понижение передачи
TEST_CASE("SwitchGear downshift from third to second succeeds at overlapping speed")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);
    gearbox.SwitchGear(2, 30, Direction::Forward);
    gearbox.SwitchGear(3, 30, Direction::Forward);

    REQUIRE(gearbox.SwitchGear(2, 30, Direction::Forward));
    REQUIRE(gearbox.GetCurrentGear().number == 2);
}

TEST_CASE("SwitchGear downshift from third to second fails when speed is too high for second")
{
    Gearbox gearbox;
    gearbox.SwitchGear(1, 0, Direction::Still);
    gearbox.SwitchGear(2, 50, Direction::Forward);
    gearbox.SwitchGear(3, 50, Direction::Forward);

    REQUIRE_FALSE(gearbox.SwitchGear(2, 51, Direction::Forward));
    REQUIRE(gearbox.GetCurrentGear().number == 3);
}