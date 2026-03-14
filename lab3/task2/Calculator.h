#ifndef OOP_CALCULATOR_H
#define OOP_CALCULATOR_H
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "IValue.h"
#include "Operation.h"


class Calculator
{
public:
    void DeclareVariable(const std::string& name);
    void LetAssign(const std::string& name, double value);
    void LetAssign(const std::string& name, const std::string& source);
    void DeclareFunction(const std::string& name,
        const std::string& leftOperand,
        std::optional<Operation> operation,
        std::optional<std::string> rightOperand);
    void Print(std::ostream& out, const std::string& name) const;
    void PrintVars(std::ostream& out) const;
    void PrintFns(std::ostream& out) const;

private:
    void AssignValueToVariable(const std::string& name, double value);
    std::map<std::string, std::unique_ptr<IValue>> m_identifiers = {};
};


#endif //OOP_CALCULATOR_H