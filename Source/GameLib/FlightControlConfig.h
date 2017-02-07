#pragma once

#include <glm/glm.hpp>

#include "GameLib.h"

struct FlightControlComponent
{
    float maxSpeed = 0.0f;
    float acceleration = 0.0f;
};

struct FlightControlDirection
{
    float acceleration = 0.0f;
    float maxSpeed = 0.0f;
};

struct FlightControlConfig
{
    FlightControlComponent forward;
    FlightControlComponent backward;
    FlightControlComponent horizontal;
    FlightControlComponent vertical;
    FlightControlComponent angular;

    FlightControlDirection direction(const glm::vec3 & direction) const;
    glm::vec3 clampAcceleration(const glm::vec3 & acceleration) const;
    glm::vec3 clampVelocity(const glm::vec3 & velocity) const;
};