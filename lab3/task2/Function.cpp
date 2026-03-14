#include "Function.h"

#include <cmath>

Function::Function(const IdentifierResolver& resolver,
                   const std::string& leftOperand,
                   const std::optional<Operation> operation,
                   const std::optional<std::string>& rightOperand)
        : m_resolver(resolver)
        , m_leftOperand(leftOperand)
        , m_operation(operation)
        , m_rightOperand(rightOperand)
{
}

double Function::GetValue() const
{
    const auto leftValue = m_resolver(m_leftOperand);
    if (!m_operation.has_value())
    {
        return leftValue;
    }

    const auto rightValue = m_resolver(*m_rightOperand);
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
        if (rightValue == 0)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return leftValue / rightValue;
    }

    return std::numeric_limits<double>::quiet_NaN();
}