cmake_minimum_required(VERSION 4.1.2)
project(task3)

find_package(Catch2 3 REQUIRED)

add_executable(freq Freq.cpp
        WordFrequency.h
        WordFrequency.cpp)

add_executable(tests3 Tests.cpp WordFrequency.cpp)

target_link_libraries(tests3 PRIVATE Catch2::Catch2WithMain)