cmake_minimum_required(VERSION 3.20)
project(task1)

add_executable(FindMaxEx Task1Main.cpp
        Athlete.h
        FindMaxEx.h)

add_executable(FindMaxExTests tests/FindMaxExTests.cpp
        Athlete.h
        FindMaxEx.h)

target_link_libraries(FindMaxExTests PRIVATE Catch2::Catch2WithMain)