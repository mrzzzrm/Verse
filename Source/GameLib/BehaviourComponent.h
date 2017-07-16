#pragma once

#include <vector>
#include <string>

#include <Deliberation/ECS/Component.h>

#include "GameLib.h"

class AbstractBehaviour;

class BehaviourComponent:
    public Component<BehaviourComponent>
{
    DELIBERATION_COMPONENT_NAME("Behaviour")

public:
    const std::vector<std::shared_ptr<AbstractBehaviour>> & behaviours() const { return m_behaviours; }

    void addBehaviour(const std::shared_ptr<AbstractBehaviour> & behaviour);
    std::shared_ptr<AbstractBehaviour> getBehaviourByName(const std::string & name);

private:
    std::vector<std::shared_ptr<AbstractBehaviour>> m_behaviours;
};