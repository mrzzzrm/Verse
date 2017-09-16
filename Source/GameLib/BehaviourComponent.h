#pragma once

#include <string>
#include <vector>

#include <Deliberation/ECS/Component.h>

#include "GameLib.h"

class AbstractEntityBehaviour;

class BehaviourComponent : public Component<BehaviourComponent>
{
    DELIBERATION_COMPONENT_NAME("Behaviour")

public:
    const std::vector<std::shared_ptr<AbstractEntityBehaviour>> & behaviours() const
    {
        return m_behaviours;
    }

    void addBehaviour(const std::shared_ptr<AbstractEntityBehaviour> & behaviour);
    std::shared_ptr<AbstractEntityBehaviour>
    getBehaviourByName(const std::string & name);

private:
    std::vector<std::shared_ptr<AbstractEntityBehaviour>> m_behaviours;
};