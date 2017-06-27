#include "FlightControlConfigPrototype.h"

#include <Deliberation/ECS/Entity.h>

static void from_json(const Json& j, FlightControlComponent & component)
{
    component.acceleration = j[0];
    component.maxSpeed = j[1];
}

void FlightControlConfigPrototype::updateComponent(FlightControlConfig & component) 
{
    component.horizontal = m_json["Horizontal"];
    component.vertical = m_json["Vertical"];
    component.forward = m_json["Forward"];
    component.backward = m_json["Backward"];
    component.angular = m_json["Angular"];
}
