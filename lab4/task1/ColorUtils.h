#ifndef OOP_COLORUTILS_H
#define OOP_COLORUTILS_H

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

inline std::string FormatColor(const uint32_t color)
{
    std::ostringstream stream;
    stream << "#" << std::hex << std::setfill('0') << std::setw(6) << color;
    return stream.str();
}

#endif //OOP_COLORUTILS_H
