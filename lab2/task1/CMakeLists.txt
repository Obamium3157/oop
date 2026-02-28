cmake_minimum_required(VERSION 4.1.2)
project(task1)

find_package(Catch2 3 REQUIRED)

add_executable(vec Vec.cpp
        Numbers.h
        Numbers.cpp)

add_executable(tests Tests.cpp Numbers.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)