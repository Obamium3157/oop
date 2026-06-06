cmake_minimum_required(VERSION 3.20)
project(task1)

set(CMAKE_CXX_STANDARD 20)

set(SFML_DIR "C:/SFML-3-0-0/SFML-3.0.0/lib/cmake/SFML")
find_package(SFML 3 COMPONENTS Graphics REQUIRED
        PATHS "${SFML_DIR}" NO_DEFAULT_PATH)

set(SHAPE_SOURCES
    FindMaxEx.h
    ICanvasDrawable.h
    ICanvas.h
    IShape.h
    ISolidShape.h
    CPoint.h
    CLineSegment.cpp CLineSegment.h
    CTriangle.cpp    CTriangle.h
    CRectangle.cpp   CRectangle.h
    CCircle.cpp      CCircle.h
    GeometryHandler.cpp GeometryHandler.h
    ColorUtils.h
)

add_executable(Geometry
    Geometry.cpp
    CCanvas.cpp CCanvas.h
    ${SHAPE_SOURCES}
)

target_link_libraries(Geometry PRIVATE SFML::Graphics)

add_custom_command(TARGET Geometry POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "C:/SFML-3-0-0/SFML-3.0.0/bin"
    $<TARGET_FILE_DIR:Geometry>
)

add_executable(GeometryTests
    tests/GeometryTests.cpp
    ${SHAPE_SOURCES}
)
target_link_libraries(GeometryTests PRIVATE Catch2::Catch2WithMain)