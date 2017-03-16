#include "FlightControlConfigPrototype.h"

#include <Deliberation/ECS/Entity.h>

static void from_json(const Json& j, FlightControlComponent & component)
{
    component.acceleration = j[0];
    component.maxSpeed = j[1];
}

FlightControlConfigPrototype::FlightControlConfigPrototype(const Json & json)
{
    m_config.horizontal = json["Horizontal"];
    m_config.vertical = json["Vertical"];
    m_config.forward = json["Forward"];
    m_config.backward = json["Backward"];
    m_config.angular = json["Angular"];
}

void FlightControlConfigPrototype::applyToEntity(Entity & entity) const
{
    entity.addComponent<FlightControlConfig>(m_config);
}