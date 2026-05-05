#include "CBody.h"

double CBody::GetMass() const
{
    return GetDensity() * GetVolume();
}

bool CBody::ContainsBody(const CBody* body) const
{
    return false;
}
