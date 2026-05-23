cmake_minimum_required(VERSION 3.20)
project(task1)

add_executable(URL URL.cpp
        CHttpUrl.cpp
        CHttpUrl.h
        CUrlParsingError.h)