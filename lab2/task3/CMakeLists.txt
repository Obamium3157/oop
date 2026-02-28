cmake_minimum_required(VERSION 4.1.2)
project(task3)

find_package(Catch2 3 REQUIRED)

add_executable(freq freq.cpp
        word_frequency.h
        word_frequency.cpp)

add_executable(tests3 tests.cpp word_frequency.cpp)

target_link_libraries(tests3 PRIVATE Catch2::Catch2WithMain)