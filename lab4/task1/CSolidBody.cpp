#include "CSolidBody.h"

CSolidBody::CSolidBody(const double density)
    : m_density(density)
{
}

double CSolidBody::GetDensity() const
{
    return m_density;
}
