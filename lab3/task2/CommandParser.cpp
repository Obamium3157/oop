#include "CommandParser.h"

#include <iostream>

namespace
{
    bool TrimLeft(std::string& s)
    {
        if (const auto pos = s.find_first_not_of(' '); pos != std::string::npos)
        {
            s = s.substr(pos);
            return true;
        }

        return false;
    }

    bool TrimRight(std::string& s)
    {
        if (const auto pos = s.find_last_not_of(' '); pos != std::string::npos)
        {
            s = s.substr(0, pos + 1);
            return true;
        }

        return false;
    }

    std::pair<std::string, std::string> SplitByEqSign(const std::string& line)
    {
        const auto eqPos = line.find('=');
        if (eqPos == std::string::npos)
        {
            throw std::runtime_error("Invalid usage");
        }

        std::string name = line.substr(0, eqPos);
        std::string value = line.substr(eqPos + 1);

        return { name, value };
    }

    std::optional<double> TryParseDouble(const std::string& s)
    {
        std::size_t pos = 0;
        try
        {
            const double number = std::stod(s, &pos);
            if (pos == s.size())
            {
                return number;
            }
        }
        catch (const std::exception&) { }
        return std::nullopt;
    }

    std::optional<std::pair<std::size_t, Operation>> FindOperation(const std::string& body)
    {
        for (std::size_t i = 0; i < body.size(); i++)
        {
            if (const auto operation = CharToOperation(body[i]))
            {
                return std::make_pair(i, *operation);
            }
        }
        return std::nullopt;
    }
}

CommandParser::CommandParser(std::istream& in, std::ostream& out, Calculator& calculator)
    : m_in(in)
    , m_out(out)
    , m_calculator(calculator)
    , m_commands({
        {"var", [this](std::istream& args) { DeclareVariable(args); }},
        {"let", [this](std::istream& args) { LetAssign(args); }},
        {"fn", [this](std::istream& args) { DeclareFunction(args); }},
        {"print", [this](std::istream& args) { Print(args); }},
        {"printvars", [this](std::istream&) { PrintVars(); }},
        {"printfns", [this](std::istream&) { PrintFns(); }}
    })
{
}

void CommandParser::Parse(const std::string& command, std::istream& args) const
{
    const auto it = m_commands.find(command);
    if (it == m_commands.end())
    {
        m_out << "Unknown command\n";
        return;
    }

    try
    {
        it->second(args);
    }
    catch (std::exception& e)
    {
        m_out << e.what() << "\n";
    }

}

void CommandParser::DeclareVariable(std::istream& args) const
{
    if (std::string name; args >> name)
    {
        m_calculator.DeclareVariable(name);
        return;
    }

    throw std::runtime_error("Invalid usage");
}

void CommandParser::LetAssign(std::istream& args) const
{
    std::string line;
    std::getline(args, line);

    if (!TrimLeft(line))
    {
        throw std::runtime_error("Invalid usage");
    }

    auto [name, value] = SplitByEqSign(line);

    TrimLeft(name);
    TrimRight(name);
    TrimLeft(value);
    TrimRight(value);

    if (name.empty() || value.empty())
    {
        throw std::runtime_error("Invalid usage");
    }

    if (const auto number = TryParseDouble(value))
    {
        m_calculator.LetAssign(name, *number);
        return;
    }

    m_calculator.LetAssign(name, value);
}

void CommandParser::DeclareFunction(std::istream& args) const
{
    std::string line;
    std::getline(args, line);

    if (!TrimLeft(line))
    {
        throw std::runtime_error("Invalid usage");
    }

    auto [fnName, body] = SplitByEqSign(line);

    TrimLeft(fnName);
    TrimRight(fnName);
    TrimLeft(body);
    TrimRight(body);

    if (fnName.empty() || body.empty())
    {
        throw std::runtime_error("Invalid usage");
    }

    const auto found = FindOperation(body);
    if (!found)
    {
        TrimLeft(body);
        if (body.empty())
        {
            throw std::runtime_error("Invalid usage");
        }
        m_calculator.DeclareFunction(fnName, body, std::nullopt, std::nullopt);
        return;
    }

    const auto [opPos, operation] = *found;
    std::string leftOperand = body.substr(0, opPos);
    std::string rightOperand = body.substr(opPos + 1);

    TrimLeft(leftOperand);
    TrimRight(leftOperand);
    TrimLeft(rightOperand);
    TrimRight(rightOperand);

    if (leftOperand.empty() || rightOperand.empty())
    {
        throw std::runtime_error("Invalid usage");
    }

    m_calculator.DeclareFunction(fnName, leftOperand, operation, rightOperand);
}

void CommandParser::Print(std::istream& args) const
{
    if (std::string name; args >> name)
    {
        m_calculator.Print(m_out, name);
        return;
    }

    throw std::runtime_error("Invalid usage");
}

void CommandParser::PrintVars() const
{
    m_calculator.PrintVars(m_out);
}

void CommandParser::PrintFns() const
{
    m_calculator.PrintFns(m_out);
}