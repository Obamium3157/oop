cmake_minimum_required(VERSION 3.20)
project(task1)

add_executable(URL URL.cpp
        CHttpUrl.cpp
        CHttpUrl.h
        CUrlParsingError.h)

add_executable(URLTests tests/URLTests.cpp
        CHttpUrl.h
        CHttpUrl.cpp
        CUrlParsingError.h)

target_link_libraries(URLTests PRIVATE Catch2::Catch2WithMain)