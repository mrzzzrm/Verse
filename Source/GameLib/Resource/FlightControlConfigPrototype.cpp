#include "FlightControlConfigPrototype.h"

#include <Deliberation/ECS/Entity.h>

static void from_json(const Json & j, FlightControlComponent & component)
{
    component.acceleration = j[0];
    component.maxSpeed = j[1];
}

void FlightControlConfigPrototype::updateComponent(
    FlightControlConfig & component)
{
    component.horizontal = m_newJson["Horizontal"];
    component.vertical = m_newJson["Vertical"];
    component.forward = m_newJson["Forward"];
    component.backward = m_newJson["Backward"];
    component.angular = m_newJson["Angular"];
}
