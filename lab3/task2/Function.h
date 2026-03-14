#ifndef OOP_FUNCTION_H
#define OOP_FUNCTION_H

#include <functional>
#include <optional>
#include <string>

#include "IIdentifier.h"
#include "Operation.h"

class Function : public IIdentifier
{
public:
    using IdentifierResolver = std::function<double(const std::string&)>;

    Function(IdentifierResolver resolver,
        std::string leftOperand,
        const std::optional<Operation>& operation,
        const std::optional<std::string>& rightOperand);

    [[nodiscard]] double GetValue() const override;

private:
    IdentifierResolver m_resolver;
    std::string m_leftOperand;
    std::optional<Operation> m_operation;
    std::optional<std::string> m_rightOperand;
};


#endif //OOP_FUNCTION_H