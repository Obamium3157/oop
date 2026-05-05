#include "BodyCollection.h"
#include <algorithm>

void BodyCollection::AddBody(std::shared_ptr<CBody> body)
{
    m_bodies.push_back(std::move(body));
}

const std::vector<std::shared_ptr<CBody>>& BodyCollection::GetBodies() const
{
    return m_bodies;
}

std::shared_ptr<CBody> BodyCollection::FindHeaviest() const
{
    if (m_bodies.empty())
    {
        return nullptr;
    }

    return *std::ranges::max_element(m_bodies, {}, [](const auto& body)
    {
        return body->GetMass();
    });
}

std::shared_ptr<CBody> BodyCollection::FindLightestInWater() const
{
    if (m_bodies.empty())
    {
        return nullptr;
    }

    return *std::ranges::min_element(m_bodies, {}, [](const auto& body)
    {
        return (body->GetDensity() - waterDensity) * body->GetVolume();
    });
}
