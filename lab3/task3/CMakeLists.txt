cmake_minimum_required(VERSION 3.20)
project(task3)

add_executable(economy Economy.cpp
        Bank.h
        Bank.cpp
        BankOperationError.h)

add_executable(BankTests tests/TestBank.cpp Bank.h Bank.cpp BankOperationError.h)

target_link_libraries(BankTests PRIVATE Catch2::Catch2WithMain)