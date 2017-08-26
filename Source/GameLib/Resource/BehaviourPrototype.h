#pragma once

#include <Deliberation/ECS/ComponentPrototype.h>

#include "BehaviourComponent.h"

class BehaviourManager;

class BehaviourPrototype : public ComponentPrototype<BehaviourComponent>
{
public:
    BehaviourPrototype(const std::shared_ptr<BehaviourManager> & manager);

    void updateComponent(const Entity & entity, BehaviourComponent & behaviourComponent) override;

private:
    std::shared_ptr<BehaviourManager> m_manager;
};