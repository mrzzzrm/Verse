#include "CoriolisPrototype.h"

#include "CoriolisComponent.h"

CoriolisPrototype::CoriolisPrototype(const Json & json)
{
    m_angularVelocity = json["AngularVelocity"];
}

void CoriolisPrototype::applyToEntity(Entity & entity) const
{
    entity.addComponent<CoriolisComponent>(m_angularVelocity);
}