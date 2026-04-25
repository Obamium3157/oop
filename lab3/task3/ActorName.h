#ifndef OOP_ACTORNAME_H
#define OOP_ACTORNAME_H

#include <iostream>

enum class ActorName
{
    Burns,
    Homer,
    Marge,
    Bart,
    Lisa,
    Apu,
};

inline std::ostream& operator<<(std::ostream& os, const ActorName actorName)
{
    switch (actorName)
    {
    case ActorName::Burns: return os << "Burns";
    case ActorName::Homer: return os << "Homer";
    case ActorName::Marge: return os << "Marge";
    case ActorName::Bart: return os << "Bart";
    case ActorName::Lisa: return os << "Lisa";
    case ActorName::Apu: return os << "Apu";
    default: return os << "Unknown";
    }
}

#endif //OOP_ACTORNAME_H
