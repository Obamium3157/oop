#ifndef OOP_COMMANDPARSER_H
#define OOP_COMMANDPARSER_H
#include <functional>
#include <map>
#include <string>

#include "Calculator.h"


class CommandParser
{
public:
    CommandParser(std::istream& in, std::ostream& out, Calculator& calculator);
    void Parse(const std::string& command, std::istream& args) const;

private:
    void DeclareVariable(std::istream& args) const;
    void LetAssign(std::istream& args) const;
    void DeclareFunction(std::istream& args) const;
    void Print(std::istream& args) const;
    void PrintVars() const;
    void PrintFns() const;

    std::istream& m_in;
    std::ostream& m_out;
    Calculator& m_calculator;

    std::map<std::string, std::function<void(std::istream&)>> m_commands;
};

#endif //OOP_COMMANDPARSER_H