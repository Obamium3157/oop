cmake_minimum_required(VERSION 3.20)
project(oop)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(lab1)
add_subdirectory(lab2)
add_subdirectory(lab3)