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

    Function(const IdentifierResolver& resolver,
        const std::string& leftOperand,
        std::optional<Operation> operation,
        const std::optional<std::string>& rightOperand);

    double GetValue() const override;

private:
    IdentifierResolver m_resolver;
    std::string m_leftOperand;
    std::optional<Operation> m_operation;
    std::optional<std::string> m_rightOperand;
};


#endif //OOP_FUNCTION_H