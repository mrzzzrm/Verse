#include "BehaviourPrototype.h"

#include <Deliberation/Core/Log.h>
#include <Deliberation/ECS/World.h>

#include "AbstractBehaviour.h"
#include "BehaviourManager.h"

BehaviourPrototype::BehaviourPrototype(
    const std::shared_ptr<BehaviourManager> & manager)
    : m_manager(manager)
{
}

void BehaviourPrototype::updateComponent(
    const Entity & entity, BehaviourComponent & behaviourComponent)
{
    DELIBERATION_LOG_INNER_SCOPE("BehaviourPrototype")

    Assert(m_newJson.is_object());

    for (const auto & pair : Json::iterator_wrapper(m_newJson))
    {
        const auto & behaviourName = pair.key();

        auto behaviour = behaviourComponent.getBehaviourByName(behaviourName);
        if (!behaviour)
        {
            Log->info("Adding '{}' to Entity '{}'", behaviourName, entity.name());

            behaviour = m_manager->createBehaviour(behaviourName);
            behaviour->setEntity(entity);
            behaviourComponent.addBehaviour(behaviour);
        }
    }
}