cmake_minimum_required(VERSION 3.20)
project(task1)

add_executable(Geometry Geometry.cpp
    FindMaxEx.h
    IShape.h
    ISolidShape.h
    CPoint.h
    CLineSegment.cpp
    CLineSegment.h
    CTriangle.cpp
    CTriangle.h
    CRectangle.cpp
    CRectangle.h
    CCircle.cpp
    CCircle.h
    GeometryHandler.cpp
    GeometryHandler.h
    ColorUtils.h
)

add_executable(GeometryTests tests/GeometryTests.cpp
        FindMaxEx.h
        IShape.h
        ISolidShape.h
        CPoint.h
        CLineSegment.cpp
        CLineSegment.h
        CTriangle.cpp
        CTriangle.h
        CRectangle.cpp
        CRectangle.h
        CCircle.cpp
        CCircle.h
        GeometryHandler.cpp
        GeometryHandler.h
        ColorUtils.h)

target_link_libraries(GeometryTests PRIVATE Catch2::Catch2WithMain)