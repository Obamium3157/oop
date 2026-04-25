cmake_minimum_required(VERSION 3.20)
project(task3)

add_executable(economy Economy.cpp
        Bank.h
        Bank.cpp
        BankOperationError.h
        IActor.h
        Burns.cpp
        Burns.h
        Homer.h
        Homer.cpp
        Bart.cpp
        Bart.h
        Lisa.cpp
        Lisa.h
        Apu.cpp
        Apu.h
        Marge.cpp
        EconomyHelper.cpp
        EconomyHelper.h
        SimulationContext.cpp
        SimulationContext.h
        Bankable.h
        Bankable.cpp)

add_executable(BankTests tests/TestBank.cpp Bank.h Bank.cpp BankOperationError.h)

target_link_libraries(BankTests PRIVATE Catch2::Catch2WithMain)