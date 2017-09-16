#pragma once

#include <Deliberation/ECS/Entity.h>

#include "AbstractBehaviour.h"
#include "GameLib.h"

class AbstractEntityBehaviour:
    public AbstractBehaviour
{
public:
    explicit AbstractEntityBehaviour(World & world): AbstractBehaviour(world) {}

    const Entity & entity() const { return m_entity; }
    void           setEntity(const Entity & entity);

protected:
    virtual void onEntityChanged() {}

protected:
    Entity      m_entity;
};