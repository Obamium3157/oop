#ifndef OOP_OPERATOR_H
#define OOP_OPERATOR_H


enum class Operation
{
    Plus,
    Minus,
    Multiply,
    Divide,
};

inline char OperationToChar(const Operation operation)
{
    switch (operation)
    {
    case Operation::Plus:     return '+';
    case Operation::Minus:    return '-';
    case Operation::Multiply: return '*';
    case Operation::Divide:   return '/';
    }
    return '\0';
}

inline std::optional<Operation> CharToOperation(const char c)
{
    switch (c)
    {
    case '+': return Operation::Plus;
    case '-': return Operation::Minus;
    case '*': return Operation::Multiply;
    case '/': return Operation::Divide;
    default:  return std::nullopt;
    }
}

#endif //OOP_OPERATOR_H