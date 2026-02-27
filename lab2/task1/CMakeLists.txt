cmake_minimum_required(VERSION 4.1.2)
project(task1)

find_package(Catch2 3 REQUIRED)

add_executable(vec vec.cpp
        numbers.h
        numbers.cpp)

add_executable(tests tests.cpp numbers.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)