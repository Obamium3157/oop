#ifndef OOP_CONSOLEHANDLER_H
#define OOP_CONSOLEHANDLER_H

#include <functional>
#include <istream>
#include <map>
#include <ostream>
#include <string>

#include "BodyCollection.h"

class ConsoleHandler
{
public:
    ConsoleHandler(std::istream& input, std::ostream& output);

    void FillCollection(BodyCollection& collection);
    void PrintResults(const BodyCollection& collection) const;

private:
    std::shared_ptr<CBody> ReadBody(const std::string& type);
    std::shared_ptr<CBody> ReadCompound();

    using BodyReader = std::function<std::shared_ptr<CBody>()>;

    static const std::string endString;
    std::map<std::string, BodyReader> m_bodyReaders;

    std::istream& m_in;
    std::ostream& m_out;
};

#endif //OOP_CONSOLEHANDLER_H
