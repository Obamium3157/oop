cmake_minimum_required(VERSION 3.20)
project(task2)

add_executable(String StringProgram.cpp
        CMyString.h
        CMyString.cpp)

add_executable(MyStringTests tests/CMyStringTests.cpp
        CMyString.h
        CMyString.cpp)

target_link_libraries(MyStringTests PRIVATE Catch2::Catch2WithMain)