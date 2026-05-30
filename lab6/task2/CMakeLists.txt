cmake_minimum_required(VERSION 3.20)
project(task2)

add_executable(StringList StringList.cpp
        CStringList.cpp
        CStringList.h)

add_executable(StringListTests CStringList.cpp
    CStringList.h
    tests/StringListTests.cpp)

target_link_libraries(StringListTests PRIVATE Catch2::Catch2WithMain)