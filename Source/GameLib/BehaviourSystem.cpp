#include "BehaviourSystem.h"

#include "AbstractBehaviour.h"
#include "BehaviourComponent.h"

BehaviourSystem::BehaviourSystem(World & world)
    : Base(world, ComponentFilter::requires<BehaviourComponent>())
    , m_manager(std::make_shared<BehaviourManager>())
{
}

void BehaviourSystem::onEntityAdded(Entity & entity)
{
    auto & behaviourComponent = entity.component<BehaviourComponent>();
    for (auto & behaviour : behaviourComponent.behaviours())
    {
        behaviour->onBehaviourStarted();
    }
}

void BehaviourSystem::onEntityGameUpdate(Entity & entity, float seconds)
{
    auto & behaviourComponent = entity.component<BehaviourComponent>();
    for (auto & behaviour : behaviourComponent.behaviours())
    {
        behaviour->onBehaviourUpdate(seconds);
    }
}