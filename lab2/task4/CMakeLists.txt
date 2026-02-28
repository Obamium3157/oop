cmake_minimum_required(VERSION 4.1.2)
project(task4)

find_package(Catch2 3 REQUIRED)

add_executable(primes primes.cpp
        prime_numbers.cpp
        prime_numbers.h)
target_compile_options(primes PRIVATE -O2)

add_executable(tests4 tests.cpp prime_numbers.cpp)
target_compile_options(tests4 PRIVATE -O2)

target_link_libraries(tests4 PRIVATE Catch2::Catch2WithMain)