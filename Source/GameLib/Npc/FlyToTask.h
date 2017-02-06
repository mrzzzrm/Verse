#pragma once

#include <glm/glm.hpp>

#include "NpcTask.h"
#include "Player/PlayerFlightControl.h"

class FlyToTask:
    public NpcTask
{
public:
    FlyToTask(std::shared_ptr<NpcFlightControl> flightControl, const glm::vec3 & destination);

    const glm::vec3 & destination() const;
    void setDestination(const glm::vec3 & destination);

    void update(float seconds) override;

private:
    glm::vec3 m_destination;
};