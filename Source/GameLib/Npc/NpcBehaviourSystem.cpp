#include "NpcBehaviourSystem.h"

#include <Deliberation/ECS/World.h>

#include "Allegiance.h"
#include "FactionManager.h"
#include "NpcAttackTask.h"
#include "NpcBehaviour.h"
#include "NpcController.h"

NpcBehaviourSystem::NpcBehaviourSystem(World & world):
    Base(world, ComponentFilter::requires<NpcBehaviour, Allegiance, NpcController>())
{}

void NpcBehaviourSystem::onEntityUpdate(Entity & entity, float seconds)
{
    auto & npcController = entity.component<NpcController>();

    if (!npcController.task())
    {
        auto task = std::make_shared<NpcAttackTask>();
        npcController.setTask(task);
    }

    auto task = npcController.task();
    auto attackTask = std::dynamic_pointer_cast<NpcAttackTask>(task);

    if (!attackTask->target().isValid())
    {
        const auto & factionManager = world().systemRef<FactionManager>();
        const auto & playerFaction = factionManager.faction("Player");

        if (!playerFaction.empty())
        {
            attackTask->setTarget(playerFaction.front());
        }
    }
}