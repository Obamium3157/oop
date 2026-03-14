cmake_minimum_required(VERSION 4.1.2)
project(task2)

find_package(Catch2 3 REQUIRED)

add_executable(calculator RunCalc.cpp
        IValue.h
        Variable.h
        Function.h
        Function.cpp
        Operation.h
        Calculator.cpp
        Calculator.h
        Variable.cpp
        CommandParser.cpp
        CommandParser.h)

add_executable(VariableTests tests/VariableTests.cpp Variable.h Variable.cpp IValue.h)
add_executable(FunctionTests tests/FunctionTests.cpp Function.h Function.cpp IValue.h Variable.h Variable.cpp)
add_executable(CalculatorTests
        tests/CalculatorTests.cpp
        Calculator.h
        Calculator.cpp
        Variable.h
        Variable.cpp
        Function.h
        Function.cpp)

target_link_libraries(VariableTests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(FunctionTests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(CalculatorTests PRIVATE Catch2::Catch2WithMain)