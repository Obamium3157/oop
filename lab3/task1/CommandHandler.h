#ifndef OOP_COMMANDHANDLER_H
#define OOP_COMMANDHANDLER_H
#include <functional>
#include <iosfwd>
#include <map>

#include "Car.h"


class CommandHandler
{
public:
    CommandHandler(std::istream& in, std::ostream& out, Car& car);
    void Handle(const std::string& command, std::istream& args) const;

private:
    void Info() const;
    void EngineOn() const;
    void EngineOff() const;
    void SetGear(std::istream& args) const;
    void SetSpeed(std::istream& args) const;
    void Help() const;

    std::istream& m_in;
    std::ostream& m_out;
    Car& m_car;

    const std::map<std::string, std::function<void(std::istream&)>> m_commands;
};


#endif //OOP_COMMANDHANDLER_H