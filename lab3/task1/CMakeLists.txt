cmake_minimum_required(VERSION 3.20)
project(task1)

add_executable(automobile Automobile.cpp
        Car.h
        Car.cpp
        Gearbox.h
        Gearbox.cpp
        Speed.h
        Simulation.h
        Simulation.cpp
        CommandHandler.h
        CommandHandler.cpp
)

add_executable(CarTests CarTests.cpp Car.cpp Gearbox.cpp)
add_executable(GearboxTests GearboxTests.cpp Gearbox.cpp)

target_link_libraries(CarTests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(GearboxTests PRIVATE Catch2::Catch2WithMain)