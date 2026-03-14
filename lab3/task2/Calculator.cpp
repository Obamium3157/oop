#include "Calculator.h"

#include <cmath>
#include <iomanip>

#include "Function.h"
#include "Variable.h"

namespace
{
    void PrintValue(std::ostream& out, const double value)
    {
        if (std::isnan(value))
        {
            out << "nan\n";
            return;
        }
        out << std::fixed << std::setprecision(2) << value << "\n";
    }

    bool IsValidIdentifier(const std::string& name)
    {
        if (name.empty() || std::isdigit(name[0]))
        {
            return false;
        }

        for (const char c : name)
        {
            if (!std::isalnum(c) && c != '_')
            {
                return false;
            }
        }

        return true;
    }
}

void Calculator::DeclareVariable(const std::string& name)
{
    if (!IsValidIdentifier(name))
    {
        throw std::runtime_error("Invalid usage");
    }
    if (m_identifiers.contains(name))
    {
        throw std::runtime_error("Name already exists");
    }
    m_identifiers[name] = std::make_unique<Variable>();
}

void Calculator::LetAssign(const std::string& name, const double value)
{
    if (!IsValidIdentifier(name))
    {
        throw std::runtime_error("Invalid usage");
    }
    AssignValueToVariable(name, value);
}

void Calculator::LetAssign(const std::string& name, const std::string& source)
{
    if (!IsValidIdentifier(name))
    {
        throw std::runtime_error("Invalid usage");
    }

    const auto sourceIt = m_identifiers.find(source);
    if (sourceIt == m_identifiers.end())
    {
        throw std::runtime_error("Name does not exist");
    }

    AssignValueToVariable(name, sourceIt->second->GetValue());
}

void Calculator::DeclareFunction(const std::string& name,
    const std::string& leftOperand,
    std::optional<Operation> operation,
    std::optional<std::string> rightOperand)
{
    if (!IsValidIdentifier(name))
    {
        throw std::runtime_error("Invalid usage");
    }
    if (m_identifiers.contains(name))
    {
        throw std::runtime_error("Name already exists");
    }
    if (!m_identifiers.contains(leftOperand))
    {
        throw std::runtime_error("Name does not exist");
    }
    if (rightOperand && !m_identifiers.contains(*rightOperand))
    {
        throw std::runtime_error("Name does not exist");
    }

    const IValue* left = m_identifiers.at(leftOperand).get();
    const IValue* right = rightOperand
        ? m_identifiers.at(*rightOperand).get()
        : nullptr;

    m_identifiers[name] = std::make_unique<Function>(left, operation, right);
}

void Calculator::Print(std::ostream& out, const std::string& name) const
{
    const auto it = m_identifiers.find(name);
    if (it == m_identifiers.end())
    {
        throw std::runtime_error("Name does not exist");
    }
    PrintValue(out, it->second->GetValue());
}

void Calculator::PrintVars(std::ostream& out) const
{
    for (const auto& [name, identifier] : m_identifiers)
    {
        if (dynamic_cast<Variable*>(identifier.get()))
        {
            out << name << ":";
            PrintValue(out, identifier->GetValue());
        }
    }
}

void Calculator::PrintFns(std::ostream& out) const
{
    for (const auto& [name, identifier] : m_identifiers)
    {
        if (dynamic_cast<Function*>(identifier.get()))
        {
            out << name << ":";
            PrintValue(out, identifier->GetValue());
        }
    }
}

void Calculator::AssignValueToVariable(const std::string& name, const double value)
{
    if (const auto it = m_identifiers.find(name); it != m_identifiers.end())
    {
        if (dynamic_cast<Function*>(it->second.get()))
        {
            throw std::runtime_error("Invalid usage");
        }
        dynamic_cast<Variable*>(it->second.get())->SetValue(value);
        return;
    }

    auto variable = std::make_unique<Variable>();
    variable->SetValue(value);
    m_identifiers[name] = std::move(variable);
}