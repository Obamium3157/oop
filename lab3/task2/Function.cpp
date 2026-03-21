#include "Function.h"

#include <cassert>
#include <cmath>

Function::Function(const IValue* leftOperand,
                   const std::optional<Operation>& operation,
                   const IValue* rightOperand)
        : m_leftOperand(leftOperand)
        , m_operation(operation)
        , m_rightOperand(rightOperand)
{
    assert(m_leftOperand != nullptr);
    assert(m_operation.has_value() == (m_rightOperand != nullptr));
}

double Function::GetValue() const
{
    const double leftValue = m_leftOperand->GetValue();

    if (!m_operation.has_value())
    {
        return leftValue;
    }

    const double rightValue = m_rightOperand->GetValue();

    if (std::isnan(leftValue) || std::isnan(rightValue))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    switch (*m_operation)
    {
    case Operation::Plus:
        return leftValue + rightValue;
    case Operation::Minus:
        return leftValue - rightValue;
    case Operation::Multiply:
        return leftValue * rightValue;
    case Operation::Divide:
        if (rightValue == 0.0)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return leftValue / rightValue;
    }

    return std::numeric_limits<double>::quiet_NaN();
}