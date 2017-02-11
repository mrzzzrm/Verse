#pragma once

#include <experimental/optional>
#include <glm/glm.hpp>

#include "NpcFlightControl.h"

class NpcSteering
{
public:
    const glm::vec3 & destination() const;

    void setDestination(const glm::vec3 & destination);
    void setStopAtDestination(bool stopAtDestination);

    void update(NpcFlightControl & flightControl, float seconds);

private:
    std::experimental::optional<glm::vec3>
                                    m_destination;
    bool                            m_stopAtDestination = true;
};