#include "SimulationContext.h"

#include "Burns.h"
#include "Homer.h"
#include "Marge.h"
#include "Bart.h"
#include "Lisa.h"
#include "Apu.h"

SimulationContext::SimulationContext(Bank& bank)
{
    auto createAndRegister = [&](auto actorPtr)
    {
        auto& ref = *actorPtr;
        m_actors.push_back(&ref);
        m_actorsByName.emplace(ref.GetName(), std::move(actorPtr));
    };

    createAndRegister(std::make_unique<Burns>(bank, *this, 0));
    createAndRegister(std::make_unique<Homer>(bank, *this, 0));
    createAndRegister(std::make_unique<Marge>(bank, *this, 0));
    createAndRegister(std::make_unique<Bart>(*this, 0));
    createAndRegister(std::make_unique<Lisa>(*this, 0));
    createAndRegister(std::make_unique<Apu>(bank, *this, 0));

    const auto burnsAccountId = m_actorsByName.at("Burns")->GetBankAccountId();
    bank.DepositMoney(*burnsAccountId, 1000);
}

IActor* SimulationContext::GetActor(const std::string& name) const
{
    const auto it = m_actorsByName.find(name);
    if (it == m_actorsByName.end())
    {
        return nullptr;
    }
    return it->second.get();
}

const std::vector<IActor*>& SimulationContext::GetActors() const
{
    return m_actors;
}