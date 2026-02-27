cmake_minimum_required(VERSION 4.1.2)
project(task2)

find_package(Catch2 3 REQUIRED)

add_executable(decode decode.cpp
        html.h
        html.cpp)

add_executable(tests2 tests.cpp html.cpp)

target_link_libraries(tests2 PRIVATE Catch2::Catch2WithMain)