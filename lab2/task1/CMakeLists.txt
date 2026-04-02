cmake_minimum_required(VERSION 3.20)
project(task1)

add_executable(vec Vec.cpp
        Numbers.h
        Numbers.cpp)

add_executable(tests Tests.cpp Numbers.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)