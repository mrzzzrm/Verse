#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/Entity.h>

#include "ComponentPrototype.h"

class CoriolisPrototype:
    public ComponentPrototype
{
public:
    explicit CoriolisPrototype(const Json & json);

    void applyToEntity(Entity & entity) const override;

private:
    glm::vec3 m_angularVelocity;
};