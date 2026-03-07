cmake_minimum_required(VERSION 4.1.2)
project(task1)

find_package(Catch2 3 REQUIRED)

add_executable(automobile Automobile.cpp
        Car.h
        Car.cpp
        Gearbox.h
        Gearbox.cpp
        Speed.h
        Simulation.h
        Simulation.cpp
)

add_executable(CarTests CarTests.cpp Car.cpp Gearbox.cpp)

target_link_libraries(CarTests PRIVATE Catch2::Catch2WithMain)