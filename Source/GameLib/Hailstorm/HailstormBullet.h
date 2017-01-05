#pragma once

#include <limits>

#include <glm/glm.hpp>

#include <Deliberation/Core/Chrono.h>

#include "GameLib.h"

using HailstormBulletID = u32;

constexpr HailstormBulletID INVALID_HAILSTORM_BULLET_ID = std::numeric_limits<HailstormBulletID>::max();

struct HailstormBullet
{
    HailstormBullet(const glm::vec3 & origin, const glm::vec3 & velocity, float energy, DurationMillis lifetime);

    glm::vec3           origin;
    glm::vec3           velocity;
    float               energy = 0.0f;
    DurationMillis      lifetime = 0;
    TimestampMicros     timestamp = 0;
    HailstormBulletID   id = INVALID_HAILSTORM_BULLET_ID;
};

#include "HailstormBullet.inl"