#ifndef OOP_FUNCTION_H
#define OOP_FUNCTION_H

#include <optional>

#include "IValue.h"
#include "Operation.h"

class Function : public IValue
{
public:
    Function(const IValue* leftOperand,
             const std::optional<Operation>& operation,
             const IValue* rightOperand);

    double GetValue() const override;

private:
    const IValue* m_leftOperand;
    std::optional<Operation> m_operation;
    const IValue* m_rightOperand;
};


#endif //OOP_FUNCTION_H
