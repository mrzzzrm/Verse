#include "BehaviourPrototype.h"

#include <Deliberation/ECS/World.h>

#include "AbstractBehaviour.h"
#include "BehaviourManager.h"

BehaviourPrototype::BehaviourPrototype(
    const std::shared_ptr<BehaviourManager> & manager)
    : m_manager(manager)
{
}

void BehaviourPrototype::updateComponent(
    BehaviourComponent & behaviourComponent)
{
    Assert(m_newJson.is_object(), "");

    auto entity = ((World *)behaviourComponent.world())
                      ->entityById(behaviourComponent.entityId());

    for (const auto & pair : Json::iterator_wrapper(m_newJson))
    {
        const auto & behaviourName = pair.key();

        auto behaviour = behaviourComponent.getBehaviourByName(behaviourName);
        if (!behaviour)
        {
            std::cout << "BehaviourPrototype: Adding '" << behaviourName
                      << "' to Entity '" << entity.name() << "'" << std::endl;

            behaviour = m_manager->createBehaviour(behaviourName);
            behaviour->setEntity(entity);
            behaviourComponent.addBehaviour(behaviour);
        }
    }
}