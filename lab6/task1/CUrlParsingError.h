#ifndef OOP_CURLPARSINGERROR_H
#define OOP_CURLPARSINGERROR_H

#include <stdexcept>
#include <string>

class CUrlParsingError : public std::invalid_argument
{
public:
    explicit CUrlParsingError(std::string const& message)
        : std::invalid_argument(message)
    {
    }
};

#endif //OOP_CURLPARSINGERROR_H
