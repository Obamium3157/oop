#ifndef OOP_BANKOPERATIONERROR_H
#define OOP_BANKOPERATIONERROR_H
#include <stdexcept>

class BankOperationError : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

#endif //OOP_BANKOPERATIONERROR_H