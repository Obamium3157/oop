cmake_minimum_required(VERSION 3.20)
project(task2)

add_executable(decode Decode.cpp
        Html.h
        Html.cpp)

add_executable(tests2 Tests.cpp Html.cpp)

target_link_libraries(tests2 PRIVATE Catch2::Catch2WithMain)