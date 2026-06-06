#include <numbers>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../CCircle.h"
#include "../CLineSegment.h"
#include "../CRectangle.h"
#include "../CTriangle.h"

using Catch::Approx;
using Catch::Matchers::ContainsSubstring;

TEST_CASE("Area is always zero", "[CLineSegment]")
{
    CLineSegment line({ 0.0, 0.0 }, { 10.0, 5.0 }, 0xff0000);
    REQUIRE(line.GetArea() == Approx(0.0));
}

TEST_CASE("Perimeter is Euclidean distance between endpoints", "[CLineSegment]")
{
    SECTION("horizontal")
    {
        CLineSegment line({ 0.0, 0.0 }, { 5.0, 0.0 }, 0);
        REQUIRE(line.GetPerimeter() == Approx(5.0));
    }

    SECTION("vertical")
    {
        CLineSegment line({ 0.0, 0.0 }, { 0.0, 7.0 }, 0);
        REQUIRE(line.GetPerimeter() == Approx(7.0));
    }

    SECTION("3-4-5 diagonal")
    {
        CLineSegment line({ 0.0, 0.0 }, { 3.0, 4.0 }, 0);
        REQUIRE(line.GetPerimeter() == Approx(5.0));
    }

    SECTION("endpoints with negative coordinates")
    {
        CLineSegment line({ -3.0, -4.0 }, { 0.0, 0.0 }, 0);
        REQUIRE(line.GetPerimeter() == Approx(5.0));
    }
}

TEST_CASE("Endpoints are stored correctly", "[CLineSegment]")
{
    CLineSegment line({ 1.5, 2.5 }, { 3.5, 4.5 }, 0);
    REQUIRE(line.GetStartPoint().x == Approx(1.5));
    REQUIRE(line.GetStartPoint().y == Approx(2.5));
    REQUIRE(line.GetEndPoint().x == Approx(3.5));
    REQUIRE(line.GetEndPoint().y == Approx(4.5));
}

TEST_CASE("ToString contains shape name, endpoints, length and outline color", "[CLineSegment]")
{
    CLineSegment line({ 0.0, 0.0 }, { 3.0, 4.0 }, 0x123321);
    const auto text = line.ToString();
    REQUIRE_THAT(text, ContainsSubstring("Line Segment"));
    REQUIRE_THAT(text, ContainsSubstring("0.00, 0.00"));
    REQUIRE_THAT(text, ContainsSubstring("3.00, 4.00"));
    REQUIRE_THAT(text, ContainsSubstring("5.00"));
    REQUIRE_THAT(text, ContainsSubstring("#123321"));
}

TEST_CASE("ToString does not contain fill color", "[CLineSegment]")
{
    CLineSegment line({ 0.0, 0.0 }, { 1.0, 0.0 }, 0xff0000);
    REQUIRE_THAT(line.ToString(), !ContainsSubstring("Fill"));
}

// --------------------------------------------------

TEST_CASE("Area is computed correctly", "[CTriangle]")
{
    SECTION("case 1")
    {
        CTriangle triangle({ 0.0, 0.0 }, { 3.0, 0.0 }, { 0.0, 4.0 }, 0, 0);
        REQUIRE(triangle.GetArea() == Approx(6.0));
    }

    SECTION("case 2")
    {
        CTriangle triangle({ 0.0, 0.0 }, { 1.0, 0.0 }, { 0.0, 1.0 }, 0, 0);
        REQUIRE(triangle.GetArea() == Approx(0.5));
    }

    SECTION("collinear points produce zero area")
    {
        CTriangle triangle({ 0.0, 0.0 }, { 1.0, 1.0 }, { 2.0, 2.0 }, 0, 0);
        REQUIRE(triangle.GetArea() == Approx(0.0));
    }
}

TEST_CASE("Perimeter is sum of three sides", "[CTriangle]")
{
    CTriangle triangle({ 0.0, 0.0 }, { 3.0, 0.0 }, { 0.0, 4.0 }, 0, 0);
    REQUIRE(triangle.GetPerimeter() == Approx(12.0));
}

TEST_CASE("Stores vertices correctly", "[CTriangle]")
{
    CTriangle triangle({ 1.0, 2.0 }, { 3.0, 4.0 }, { 5.0, 6.0 }, 0, 0);
    REQUIRE(triangle.GetVertex1().x == Approx(1.0));
    REQUIRE(triangle.GetVertex1().y == Approx(2.0));
    REQUIRE(triangle.GetVertex2().x == Approx(3.0));
    REQUIRE(triangle.GetVertex2().y == Approx(4.0));
    REQUIRE(triangle.GetVertex3().x == Approx(5.0));
    REQUIRE(triangle.GetVertex3().y == Approx(6.0));
}

TEST_CASE("Stores colors correctly", "[CTriangle]")
{
    CTriangle triangle({ 0.0, 0.0 }, { 1.0, 0.0 }, { 0.0, 1.0 }, 0xff0000, 0x00ff00);
    REQUIRE(triangle.GetOutlineColor() == 0xff0000);
    REQUIRE(triangle.GetFillColor() == 0x00ff00);
}

TEST_CASE("ToString contains shape name, area, perimeter and both colors", "[CTriangle]")
{
    CTriangle triangle({ 0.0, 0.0 }, { 3.0, 0.0 }, { 0.0, 4.0 }, 0xff0000, 0x00ff00);
    const auto text = triangle.ToString();
    REQUIRE_THAT(text, ContainsSubstring("Triangle"));
    REQUIRE_THAT(text, ContainsSubstring("6.00"));
    REQUIRE_THAT(text, ContainsSubstring("12.00"));
    REQUIRE_THAT(text, ContainsSubstring("#ff0000"));
    REQUIRE_THAT(text, ContainsSubstring("#00ff00"));
}

// ----------------------------------

TEST_CASE("Area equals width times height", "[CRectangle]")
{
    SECTION("integer dimensions")
    {
        CRectangle rect({ 0.0, 0.0 }, 10.0, 5.0, 0, 0);
        REQUIRE(rect.GetArea() == Approx(50.0));
    }

    SECTION("fractional dimensions")
    {
        CRectangle rect({ 0.0, 0.0 }, 2.5, 4.0, 0, 0);
        REQUIRE(rect.GetArea() == Approx(10.0));
    }
}

TEST_CASE("Perimeter equals doubled sum of sides", "[CRectangle]")
{
    CRectangle rect({ 0.0, 0.0 }, 4.0, 3.0, 0, 0);
    REQUIRE(rect.GetPerimeter() == Approx(14.0));
}

TEST_CASE("Right-bottom corner is derived from left-top and dimensions", "[CRectangle]")
{
    CRectangle rect({ 1.0, 2.0 }, 4.0, 3.0, 0, 0);
    REQUIRE(rect.GetRightBottom().x == Approx(5.0));
    REQUIRE(rect.GetRightBottom().y == Approx(5.0));
}

TEST_CASE("Stores all geometric attributes correctly", "[CRectangle]")
{
    CRectangle rect({ 2.0, 5.0 }, 7.5, 3.2, 0, 0);
    REQUIRE(rect.GetLeftTop().x == Approx(2.0));
    REQUIRE(rect.GetLeftTop().y == Approx(5.0));
    REQUIRE(rect.GetWidth() == Approx(7.5));
    REQUIRE(rect.GetHeight() == Approx(3.2));
}

TEST_CASE("Stores colors correctly (CRectangle)", "[CRectangle]")
{
    CRectangle rect({ 0.0, 0.0 }, 1.0, 1.0, 0xabcdef, 0x654321);
    REQUIRE(rect.GetOutlineColor() == 0xabcdef);
    REQUIRE(rect.GetFillColor() == 0x654321);
}

TEST_CASE("ToString (CTriangle) contains shape name, area, perimeter and both colors", "[CRectangle]")
{
    CRectangle rect({ 0.0, 0.0 }, 4.0, 3.0, 0xff0000, 0x00ff00);
    const auto text = rect.ToString();
    REQUIRE_THAT(text, ContainsSubstring("Rectangle"));
    REQUIRE_THAT(text, ContainsSubstring("12.00"));
    REQUIRE_THAT(text, ContainsSubstring("14.00"));
    REQUIRE_THAT(text, ContainsSubstring("#ff0000"));
    REQUIRE_THAT(text, ContainsSubstring("#00ff00"));
}


// ------------------------------------------------

TEST_CASE("Area equals pi times radius squared", "[CCircle]")
{
    SECTION("unit circle")
    {
        CCircle circle({ 0.0, 0.0 }, 1.0, 0, 0);
        REQUIRE(circle.GetArea() == Approx(std::numbers::pi));
    }

    SECTION("radius 5")
    {
        CCircle circle({ 0.0, 0.0 }, 5.0, 0, 0);
        REQUIRE(circle.GetArea() == Approx(std::numbers::pi * 25.0));
    }
}

TEST_CASE("Perimeter equals 2 pi radius", "[CCircle]")
{
    SECTION("unit circle")
    {
        CCircle circle({ 0.0, 0.0 }, 1.0, 0, 0);
        REQUIRE(circle.GetPerimeter() == Approx(2.0 * std::numbers::pi));
    }

    SECTION("radius 3")
    {
        CCircle circle({ 0.0, 0.0 }, 3.0, 0, 0);
        REQUIRE(circle.GetPerimeter() == Approx(2.0 * std::numbers::pi * 3.0));
    }
}

TEST_CASE("Stores center and radius correctly", "[CCircle]")
{
    CCircle circle({ 2.5, 3.5 }, 4.0, 0, 0);
    REQUIRE(circle.GetCenter().x == Approx(2.5));
    REQUIRE(circle.GetCenter().y == Approx(3.5));
    REQUIRE(circle.GetRadius() == Approx(4.0));
}

TEST_CASE("Stores colors correctly (CCircle)", "[CCircle]")
{
    CCircle circle({ 0.0, 0.0 }, 1.0, 0xffffff, 0x000000);
    REQUIRE(circle.GetOutlineColor() == 0xffffff);
    REQUIRE(circle.GetFillColor() == 0x000000);
}

TEST_CASE("ToString contains shape name, radius and both colors", "[CCircle]")
{
    CCircle circle({ 0.0, 0.0 }, 5.0, 0xff0000, 0x00ff00);
    const auto text = circle.ToString();
    REQUIRE_THAT(text, ContainsSubstring("Circle"));
    REQUIRE_THAT(text, ContainsSubstring("5.00"));
    REQUIRE_THAT(text, ContainsSubstring("#ff0000"));
    REQUIRE_THAT(text, ContainsSubstring("#00ff00"));
}