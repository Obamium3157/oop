cmake_minimum_required(VERSION 4.1.2)
project(task2)

find_package(Catch2 3 REQUIRED)

add_executable(decode Decode.cpp
        Html.h
        Html.cpp)

add_executable(tests2 Tests.cpp Html.cpp)

target_link_libraries(tests2 PRIVATE Catch2::Catch2WithMain)