cmake_minimum_required(VERSION 3.20)
project(task3)

add_executable(freq Freq.cpp
        WordFrequency.h
        WordFrequency.cpp)

add_executable(tests3 Tests.cpp WordFrequency.cpp)

target_link_libraries(tests3 PRIVATE Catch2::Catch2WithMain)