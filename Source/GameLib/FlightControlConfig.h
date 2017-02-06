#pragma once

#include <glm/glm.hpp>

#include "GameLib.h"

struct FlightControlComponent
{
    float maxSpeed = 0.0f;
    float acceleration = 0.0f;
};

struct FlightControlFrame
{
    float acceleration = 0.0f;
    float maxSpeed = 0.0f;
    glm::vec3 normalized;
    glm::vec3 velocityClamped;
    glm::vec3 accelerationClamped;
};

struct FlightControlConfig
{
    FlightControlComponent forward;
    FlightControlComponent backward;
    FlightControlComponent horizontal;
    FlightControlComponent vertical;
    FlightControlComponent angular;

    FlightControlFrame frame(const glm::vec3 & direction) const;
    glm::vec3 clampAcceleration(const glm::vec3 & acceleration) const;
    glm::vec3 clampVelocity(const glm::vec3 & velocity) const;
};