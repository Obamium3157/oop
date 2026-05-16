cmake_minimum_required(VERSION 3.20)
project(task2)

add_executable(String StringProgram.cpp
        CMyString.h
        CMyString.cpp)

add_executable(StringTests tests/StringTests.cpp
        CMyString.h
        CMyString.cpp)

target_link_libraries(StringTests PRIVATE Catch2::Catch2WithMain)