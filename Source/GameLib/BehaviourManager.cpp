#include "BehaviourManager.h"

#include "LaunchDefenseBehaviour.h"

BehaviourManager::BehaviourManager()
{
    addBehaviourType<LaunchDefenseBehaviour>("LaunchDefense");
}

std::shared_ptr<AbstractBehaviour> BehaviourManager::createBehaviour(const std::string & name)
{
    const auto iter = m_behaviourFactoryByName.find(name);
    Assert(iter != m_behaviourFactoryByName.end(), "No such Behaviour '" + name + "'");

    return iter->second();
}
