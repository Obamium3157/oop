#include "ConsoleHandler.h"

#include "CCompound.h"
#include "CCone.h"
#include "CCylinder.h"
#include "CParallelepiped.h"
#include "CSphere.h"

const std::string ConsoleHandler::endString = "end";

ConsoleHandler::ConsoleHandler(std::istream& input, std::ostream& output)
    : m_in(input)
    , m_out(output)
{
    m_bodyReaders["sphere"] = [this]
    {
        double density, radius;
        m_in >> density >> radius;
        return std::make_shared<CSphere>(density, radius);
    };

    m_bodyReaders["cone"] = [this]
    {
        double density, baseRadius, height;
        m_in >> density >> baseRadius >> height;
        return std::make_shared<CCone>(density, baseRadius, height);
    };

    m_bodyReaders["cylinder"] = [this]
    {
        double density, baseRadius, height;
        m_in >> density >> baseRadius >> height;
        return std::make_shared<CCylinder>(density, baseRadius, height);
    };

    m_bodyReaders["parallelepiped"] = [this]
    {
        double density, width, height, depth;
        m_in >> density >> width >> height >> depth;
        return std::make_shared<CParallelepiped>(density, width, height, depth);
    };

    m_bodyReaders["compound"] = [this]
    {
        return ReadCompound();
    };
}

void ConsoleHandler::FillCollection(BodyCollection& collection)
{
    std::string type;
    while (m_in >> type && type != endString)
    {
        if (auto body = ReadBody(type))
        {
            collection.AddBody(std::move(body));
        }
        else
        {
            m_out << "Unknown type: " << type << "\n";
        }
    }
}

void ConsoleHandler::PrintResults(const BodyCollection& collection) const
{
    const auto& bodies = collection.GetBodies();

    m_out << "All bodies (" << bodies.size() << "):\n";
    for (const auto& body : bodies)
    {
        m_out << body->ToString() << "\n\n";
    }

    if (const auto heaviest = collection.FindHeaviest())
    {
        m_out << "Heaviest body: " << heaviest->ToString() << "\n";
    }

    if (const auto lightestInWater = collection.FindLightestInWater())
    {
        m_out << "Lightest in water: " << lightestInWater->ToString() << "\n";
    }
}

std::shared_ptr<CBody> ConsoleHandler::ReadBody(const std::string& type)
{
    const auto it = m_bodyReaders.find(type);
    if (it == m_bodyReaders.end())
    {
        return nullptr;
    }

    return it->second();
}

std::shared_ptr<CBody> ConsoleHandler::ReadCompound()
{
    auto compound = std::make_shared<CCompound>();

    std::string type;
    while (m_in >> type && type != endString)
    {
        if (const auto child = ReadBody(type))
        {
            compound->AddChildBody(child);
        }
        else
        {
            m_out << "Unknown type: " << type << "\n";
        }
    }

    return compound;
}
