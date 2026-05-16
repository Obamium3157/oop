cmake_minimum_required(VERSION 3.20)
project(task1)

add_executable(Dates Date.cpp
        CDate.h
        CDate.cpp)

add_executable(DateTests tests/DateTests.cpp
    CDate.h
    CDate.cpp)


target_link_libraries(DateTests PRIVATE Catch2::Catch2WithMain)