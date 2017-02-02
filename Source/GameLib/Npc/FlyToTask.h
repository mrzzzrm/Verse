#pragma once

#include <glm/glm.hpp>

#include "NpcTask.h"
#include "FlightControl.h"

class FlyToTask:
    public NpcTask
{
public:
    FlyToTask(std::shared_ptr<FlightControl> flightControl, const glm::vec3 & destination);

    void update(float seconds) override;

private:
    glm::vec3                       m_destination;
};