#pragma once

#include <Deliberation/ECS/Entity.h>

#include "GameLib.h"

class AbstractBehaviour
{
public:
    virtual ~AbstractBehaviour() = default;

    const std::string & name() const { return m_name; }
    void                setName(const std::string & name) { m_name = name; }

    const Entity & entity() const { return m_entity; }
    void           setEntity(const Entity & entity) { m_entity = entity; }

    virtual void onBehaviourStarted(){};
    virtual void onBehaviourUpdate(float seconds){};

protected:
    Entity      m_entity;
    std::string m_name;
};