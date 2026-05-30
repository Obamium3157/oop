cmake_minimum_required(VERSION 3.20)
project(task2)

add_executable(MyArray Task2Main.cpp
        CMyArray.h)

add_executable(MyArrayTests tests/MyArrayTests.cpp
    CMyArray.h
)

target_link_libraries(MyArrayTests PRIVATE Catch2::Catch2WithMain)