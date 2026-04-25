#ifndef OOP_SIMULATIONCONTEXT_H
#define OOP_SIMULATIONCONTEXT_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "IActor.h"

class Bank;

class SimulationContext
{
public:
    explicit SimulationContext(Bank& bank);

    [[nodiscard]] IActor* GetActor(const std::string& name) const;
    [[nodiscard]] const std::vector<IActor*>& GetActors() const;

private:
    std::map<std::string, std::unique_ptr<IActor>> m_actorsByName;
    std::vector<IActor*> m_actors;
};

#endif // OOP_SIMULATIONCONTEXT_H