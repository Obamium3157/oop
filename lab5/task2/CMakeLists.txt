cmake_minimum_required(VERSION 3.20)
project(task2)

add_executable(String StringProgram.cpp
    CString.h
    CString.cpp)

add_executable(StringTests tests/StringTests.cpp
    CString.h
    CString.cpp)

target_link_libraries(StringTests PRIVATE Catch2::Catch2WithMain)