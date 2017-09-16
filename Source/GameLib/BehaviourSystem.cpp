#include "BehaviourSystem.h"

#include "AbstractEntityBehaviour.h"
#include "BehaviourComponent.h"

BehaviourSystem::BehaviourSystem(World & world)
    : Base(world, ComponentFilter::requires<BehaviourComponent>())
    , m_manager(std::make_shared<BehaviourManager>())
{
}

void BehaviourSystem::onEntityAdded(Entity & entity)
{
//    auto & behaviourComponent = entity.component<BehaviourComponent>();
//    for (auto & behaviour : behaviourComponent.behaviours())
//    {
//        behaviour->onBehaviourStarted();
//    }
}

void BehaviourSystem::onEntityGameUpdate(Entity & entity, const UpdateFrame & updateFrame)
{
//    auto & behaviourComponent = entity.component<BehaviourComponent>();
//    for (auto & behaviour : behaviourComponent.behaviours())
//    {
//        behaviour->onBehaviourUpdate(updateFrame);
//    }
}