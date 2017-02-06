#include "FlightControlConfig.h"

#include <iostream>

#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/Math/MathUtils.h>

namespace 
{

glm::vec3 ProjectToFrame(glm::vec3 v, const glm::vec3 & frame)
{
    if (v.x < frame.x && v.x > 0.001f) v *= frame.x / v.x;
    if (v.y < frame.y && v.y > 0.001f) v *= frame.y / v.y;
    if (v.z < frame.z && v.z > 0.001f) v *= frame.z / v.z;

    if (v.x > frame.x) v /= v.x / frame.x;
    if (v.y > frame.y) v /= v.y / frame.y;
    if (v.z > frame.z) v /= v.z / frame.z;

    return v;
}

glm::vec3 ClampToFrame(glm::vec3 v, const glm::vec3 & frame)
{
    if (v.x > frame.x) v /= v.x / frame.x;
    if (v.y > frame.y) v /= v.y / frame.y;
    if (v.z > frame.z) v /= v.z / frame.z;

    return v;
}

}

FlightControlFrame FlightControlConfig::frame(const glm::vec3 & direction) const
{
    FlightControlFrame result;

    glm::vec3 mirror{
        Sign(direction.x),
        Sign(direction.y),
        Sign(direction.z)};

    auto accelClamped = direction * mirror;
    auto velocityClamped = direction * mirror;

    glm::vec3 accelFrame{
        horizontal.acceleration,
        vertical.acceleration,
        direction.z <= 0.0f ? forward.acceleration : backward.acceleration};
    glm::vec3 maxSpeedFrame{
        horizontal.maxSpeed,
        vertical.maxSpeed,
        direction.z <= 0.0f ? forward.maxSpeed : backward.maxSpeed};

    accelClamped = ProjectToFrame(accelClamped, accelFrame);
    velocityClamped = ProjectToFrame(velocityClamped, maxSpeedFrame);

    result.maxSpeed = glm::length(velocityClamped);
    result.acceleration = glm::length(accelClamped);
    result.velocityClamped = velocityClamped * mirror;
    result.accelerationClamped = accelClamped * mirror;
    result.normalized = glm::normalize(result.accelerationClamped);

    return result;
}

glm::vec3 FlightControlConfig::clampAcceleration(const glm::vec3 & acceleration) const
{
    glm::vec3 mirror{
        Sign(acceleration.x),
        Sign(acceleration.y),
        Sign(acceleration.z)};

    glm::vec3 frame{
        horizontal.acceleration,
        vertical.acceleration,
        acceleration.z <= 0.0f ? forward.acceleration : backward.acceleration};

    return ClampToFrame(acceleration * mirror, frame) * mirror;
}

glm::vec3 FlightControlConfig::clampVelocity(const glm::vec3 & velocity) const
{
    glm::vec3 mirror{
        Sign(velocity.x),
        Sign(velocity.y),
        Sign(velocity.z)};

    glm::vec3 frame{
        horizontal.maxSpeed,
        vertical.maxSpeed,
        velocity.z <= 0.0f ? forward.maxSpeed : backward.maxSpeed};

    return ClampToFrame(velocity * mirror, frame) * mirror;
}