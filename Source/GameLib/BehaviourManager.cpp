#include "BehaviourManager.h"

#include "LaunchDefenseBehaviour.h"

BehaviourManager::BehaviourManager()
{
//    addBehaviourType<LaunchDefenseBehaviour>("LaunchDefense");
}

std::shared_ptr<AbstractBehaviour>
BehaviourManager::createBehaviour(const std::string & name, World & world)
{
    const auto iter = m_behaviourFactoryByName.find(name);
    AssertM(
        iter != m_behaviourFactoryByName.end(),
        "No such Behaviour '" + name + "'");

    return iter->second(world);
}
