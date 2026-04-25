#ifndef OOP_SIMULATIONCONTEXT_H
#define OOP_SIMULATIONCONTEXT_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "IActor.h"

class Bank;

class SimulationContext
{
public:
    explicit SimulationContext(Bank& bank);


    IActor* GetActor(ActorName name) const;
    const std::vector<IActor*>& GetActors() const;
    void Iterate() const;

private:
    std::unordered_map<ActorName, std::unique_ptr<IActor>> m_actorsByName;
    std::vector<IActor*> m_actors;
};

#endif // OOP_SIMULATIONCONTEXT_H