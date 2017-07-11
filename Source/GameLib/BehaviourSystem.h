#pragma once

#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/System.h>

#include "BehaviourManager.h"
#include "GameLib.h"

class BehaviourSystem:
    public System<BehaviourSystem>
{
public:
    BehaviourSystem(World & world);

    const std::shared_ptr<BehaviourManager> & manager() { return m_manager; }
    std::shared_ptr<const BehaviourManager> manager() const { return m_manager; }

protected:
    void onEntityAdded(Entity & entity) override;
    void onEntityGameUpdate(Entity & entity, float seconds) override;

protected:
    std::shared_ptr<BehaviourManager> m_manager;
};