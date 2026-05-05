#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include <memory>
#include <string>

#include "../CCone.h"
#include "../CCompound.h"
#include "../CCylinder.h"
#include "../CParallelepiped.h"
#include "../CSphere.h"

using Catch::Approx;

TEST_CASE("CSphere constructor", "[sphere]")
{
    const CSphere sphere(1000.0, 3.0);

    REQUIRE(sphere.GetDensity() == Approx(1000.0));
    REQUIRE(sphere.GetRadius() == Approx(3.0));
}

TEST_CASE("CSphere volume =  4/3 * pi * r^3", "[sphere]")
{
    const CSphere sphere(1000.0, 3.0);
    const double expected = (4.0 / 3.0) * M_PI * 3.0 * 3.0 * 3.0;

    REQUIRE(sphere.GetVolume() == Approx(expected));
}

TEST_CASE("CSphere mass = density * volume", "[sphere]")
{
    const CSphere sphere(1000.0, 3.0);

    REQUIRE(sphere.GetMass() == Approx(sphere.GetDensity() * sphere.GetVolume()));
}

TEST_CASE("CSphere::ToString contains type name", "[sphere]")
{
    const CSphere sphere(1000.0, 3.0);

    REQUIRE(sphere.ToString().find("Sphere") != std::string::npos);
}

TEST_CASE("CSphere::ContainsBody always returns false", "[sphere]")
{
    const CSphere sphere(1000.0, 3.0);
    const CSphere other(500.0, 1.0);

    REQUIRE_FALSE(sphere.ContainsBody(&other));
    REQUIRE_FALSE(sphere.ContainsBody(&sphere));
}


TEST_CASE("CCone constructor", "[cone]")
{
    const CCone cone(2000.0, 3.0, 4.0);

    REQUIRE(cone.GetDensity() == Approx(2000.0));
    REQUIRE(cone.GetBaseRadius() == Approx(3.0));
    REQUIRE(cone.GetHeight() == Approx(4.0));
}

TEST_CASE("CCone volume = 1/3 * pi * r^2 * h", "[cone]")
{
    const CCone cone(2000.0, 3.0, 4.0);
    const double expected = (1.0 / 3.0) * M_PI * 3.0 * 3.0 * 4.0;

    REQUIRE(cone.GetVolume() == Approx(expected));
}

TEST_CASE("CCone mass = density * volume", "[cone]")
{
    const CCone cone(2000.0, 3.0, 4.0);

    REQUIRE(cone.GetMass() == Approx(cone.GetDensity() * cone.GetVolume()));
}

TEST_CASE("CCone::ToString contains type name", "[cone]")
{
    const CCone cone(2000.0, 3.0, 4.0);

    REQUIRE(cone.ToString().find("Cone") != std::string::npos);
}

TEST_CASE("CCylinder constructor", "[cylinder]")
{
    const CCylinder cylinder(500.0, 2.0, 5.0);

    REQUIRE(cylinder.GetDensity() == Approx(500.0));
    REQUIRE(cylinder.GetBaseRadius() == Approx(2.0));
    REQUIRE(cylinder.GetHeight() == Approx(5.0));
}

TEST_CASE("CCylinder volume = pi * r^2 * h", "[cylinder]")
{
    const CCylinder cylinder(500.0, 2.0, 5.0);
    const double expected = M_PI * 2.0 * 2.0 * 5.0;

    REQUIRE(cylinder.GetVolume() == Approx(expected));
}

TEST_CASE("CCylinder mass = density * volume", "[cylinder]")
{
    const CCylinder cylinder(500.0, 2.0, 5.0);

    REQUIRE(cylinder.GetMass() == Approx(cylinder.GetDensity() * cylinder.GetVolume()));
}

TEST_CASE("CCylinder::ToString contains type name", "[cylinder]")
{
    const CCylinder cylinder(500.0, 2.0, 5.0);

    REQUIRE(cylinder.ToString().find("Cylinder") != std::string::npos);
}

TEST_CASE("CParallelepiped constructor", "[parallelepiped]")
{
    const CParallelepiped box(800.0, 2.0, 3.0, 4.0);

    REQUIRE(box.GetDensity() == Approx(800.0));
    REQUIRE(box.GetWidth() == Approx(2.0));
    REQUIRE(box.GetHeight() == Approx(3.0));
    REQUIRE(box.GetDepth() == Approx(4.0));
}

TEST_CASE("CParallelepiped volume = w * h * d", "[parallelepiped]")
{
    const CParallelepiped box(800.0, 2.0, 3.0, 4.0);

    REQUIRE(box.GetVolume() == Approx(24.0));
}

TEST_CASE("CParallelepiped mass = density * volume", "[parallelepiped]")
{
    const CParallelepiped box(800.0, 2.0, 3.0, 4.0);

    REQUIRE(box.GetMass() == Approx(800.0 * 24.0));
}

TEST_CASE("CParallelepiped::ToString contains type name", "[parallelepiped]")
{
    const CParallelepiped box(800.0, 2.0, 3.0, 4.0);

    REQUIRE(box.ToString().find("Parallelepiped") != std::string::npos);
}

TEST_CASE("CCompound with no children has zero volume, mass and density", "[compound]")
{
    const CCompound compound;

    REQUIRE(compound.GetVolume() == Approx(0.0));
    REQUIRE(compound.GetMass() == Approx(0.0));
    REQUIRE(compound.GetDensity() == Approx(0.0));
}

TEST_CASE("CCompound with single child mirrors that child's properties", "[compound]")
{
    const auto sphere = std::make_shared<CSphere>(1000.0, 2.0);
    CCompound compound;
    compound.AddChildBody(sphere);

    REQUIRE(compound.GetVolume() == Approx(sphere->GetVolume()));
    REQUIRE(compound.GetMass() == Approx(sphere->GetMass()));
    REQUIRE(compound.GetDensity() == Approx(sphere->GetDensity()));
}

TEST_CASE("CCompound with multiple children aggregates correctly", "[compound]")
{
    const auto sphere = std::make_shared<CSphere>(1000.0, 1.0);
    const auto cylinder = std::make_shared<CCylinder>(500.0, 2.0, 5.0);

    CCompound compound;
    compound.AddChildBody(sphere);
    compound.AddChildBody(cylinder);

    const double expectedVolume = sphere->GetVolume() + cylinder->GetVolume();
    const double expectedMass = sphere->GetMass() + cylinder->GetMass();
    const double expectedDensity = expectedMass / expectedVolume;

    REQUIRE(compound.GetVolume() == Approx(expectedVolume));
    REQUIRE(compound.GetMass() == Approx(expectedMass));
    REQUIRE(compound.GetDensity() == Approx(expectedDensity));
}

TEST_CASE("CCompound sums volumes and masses across nesting levels", "[compound]")
{
    const auto sphere = std::make_shared<CSphere>(1000.0, 1.0);
    const auto cone = std::make_shared<CCone>(2000.0, 2.0, 3.0);
    const auto cylinder = std::make_shared<CCylinder>(500.0, 1.5, 4.0);

    const auto inner = std::make_shared<CCompound>();
    inner->AddChildBody(sphere);
    inner->AddChildBody(cone);

    const auto outer = std::make_shared<CCompound>();
    outer->AddChildBody(inner);
    outer->AddChildBody(cylinder);

    const double expectedVolume = sphere->GetVolume() + cone->GetVolume() + cylinder->GetVolume();
    const double expectedMass = sphere->GetMass() + cone->GetMass() + cylinder->GetMass();

    REQUIRE(outer->GetVolume() == Approx(expectedVolume));
    REQUIRE(outer->GetMass() == Approx(expectedMass));
}

TEST_CASE("CCompound::AddChildBody returns true for a valid child", "[compound]")
{
    const auto compound = std::make_shared<CCompound>();
    const auto sphere = std::make_shared<CSphere>(1000.0, 1.0);

    REQUIRE(compound->AddChildBody(sphere));
}

TEST_CASE("CCompound::AddChildBody rejects self-addition", "[compound]")
{
    const auto compound = std::make_shared<CCompound>();

    REQUIRE_FALSE(compound->AddChildBody(compound));
}

TEST_CASE("CCompound::AddChildBody rejects direct cycle", "[compound]")
{
    const auto outer = std::make_shared<CCompound>();
    const auto inner = std::make_shared<CCompound>();
    outer->AddChildBody(inner);

    REQUIRE_FALSE(inner->AddChildBody(outer));
}

TEST_CASE("CCompound::AddChildBody rejects indirect cycle", "[compound]")
{
    const auto c1 = std::make_shared<CCompound>();
    const auto c2 = std::make_shared<CCompound>();
    const auto c3 = std::make_shared<CCompound>();
    c1->AddChildBody(c2);
    c2->AddChildBody(c3);

    REQUIRE_FALSE(c3->AddChildBody(c1));
}

TEST_CASE("CCompound::AddChildBody does not add body after rejection", "[compound]")
{
    const auto compound = std::make_shared<CCompound>();
    compound->AddChildBody(compound);

    REQUIRE(compound->GetVolume() == Approx(0.0));
}

TEST_CASE("CCompound::ContainsBody returns true for direct child", "[compound]")
{
    const auto compound = std::make_shared<CCompound>();
    const auto sphere = std::make_shared<CSphere>(1000.0, 1.0);
    compound->AddChildBody(sphere);

    REQUIRE(compound->ContainsBody(sphere.get()));
}

TEST_CASE("CCompound::ContainsBody returns true for deeply nested child", "[compound]")
{
    const auto inner = std::make_shared<CCompound>();
    const auto deepSphere = std::make_shared<CSphere>(500.0, 1.0);
    inner->AddChildBody(deepSphere);

    const auto outer = std::make_shared<CCompound>();
    outer->AddChildBody(inner);

    REQUIRE(outer->ContainsBody(deepSphere.get()));
}

TEST_CASE("CCompound::ContainsBody returns false for unrelated body", "[compound]")
{
    const auto compound = std::make_shared<CCompound>();
    const auto sphere = std::make_shared<CSphere>(1000.0, 1.0);
    compound->AddChildBody(sphere);

    const CSphere unrelated(500.0, 2.0);
    REQUIRE_FALSE(compound->ContainsBody(&unrelated));
}

TEST_CASE("CCompound::ToString contains type name and child type names", "[compound]")
{
    const auto sphere = std::make_shared<CSphere>(1000.0, 1.0);
    const auto cylinder = std::make_shared<CCylinder>(500.0, 2.0, 3.0);

    const auto compound = std::make_shared<CCompound>();
    compound->AddChildBody(sphere);
    compound->AddChildBody(cylinder);

    const std::string result = compound->ToString();
    REQUIRE(result.find("Compound") != std::string::npos);
    REQUIRE(result.find("Sphere") != std::string::npos);
    REQUIRE(result.find("Cylinder") != std::string::npos);
}
