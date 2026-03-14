#include "Variable.h"

double Variable::GetValue() const
{
    return m_value;
}


void Variable::SetValue(const double value)
{
    m_value = value;
}
