cmake_minimum_required(VERSION 3.20)
project(task1)

add_executable(Geometry Geometry.cpp
        CBody.cpp
        CBody.h
        CSolidBody.cpp
        CSolidBody.h
        CSphere.cpp
        CSphere.h
        CCone.cpp
        CCone.h
        CCylinder.cpp
        CCylinder.h
        CParallelepiped.cpp
        CParallelepiped.h
        CCompound.cpp
        CCompound.h
        BodyCollection.cpp
        BodyCollection.h
        ConsoleHandler.cpp
        ConsoleHandler.h)

add_executable(GeometryTests tests/GeometryTests.cpp
        CBody.cpp
        CBody.h
        CSolidBody.cpp
        CSolidBody.h
        CSphere.cpp
        CSphere.h
        CCone.cpp
        CCone.h
        CCylinder.cpp
        CCylinder.h
        CParallelepiped.cpp
        CParallelepiped.h
        CCompound.cpp
        CCompound.h)

target_link_libraries(GeometryTests PRIVATE Catch2::Catch2WithMain)