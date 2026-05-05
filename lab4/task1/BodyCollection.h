#ifndef OOP_BODYCOLLECTION_H
#define OOP_BODYCOLLECTION_H

#include <memory>
#include <vector>
#include "CBody.h"

class BodyCollection
{
public:
    void AddBody(std::shared_ptr<CBody> body);

    const std::vector<std::shared_ptr<CBody>>& GetBodies() const;

    std::shared_ptr<CBody> FindHeaviest() const;
    std::shared_ptr<CBody> FindLightestInWater() const;

private:
    static constexpr double waterDensity = 1000.0;

    std::vector<std::shared_ptr<CBody>> m_bodies;
};

#endif //OOP_BODYCOLLECTION_H
