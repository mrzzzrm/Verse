#pragma once

#include <limits>

#include <glm/glm.hpp>

#include <Deliberation/Core/Chrono.h>

#include "GameLib.h"
#include "HailstormDefines.h"
#include "VoxelDefines.h"

struct HailstormBulletID
{
    HailstormBulletID(HailstormMeshID meshID,
                      u32 renderBatchIndex,
                      size_t physicsWorldIndex);

    HailstormMeshID meshID;
    u32             renderBatchIndex;
    size_t          physicsWorldIndex;
};

struct HailstormBullet
{
    HailstormBullet(const glm::vec3 & origin, const glm::vec3 & velocity, float energy, TimestampMillis birth,
                    DurationMillis lifetime, HailstormMeshID meshID, VoxelObjectWorldUID creator);

    HailstormBulletID   id;
    glm::vec3           origin;
    glm::vec3           velocity;
    float               energy = 0.0f;
    DurationMillis      lifetime = 0;
    TimestampMillis     birth = 0;
    VoxelObjectWorldUID creator = 0;
};

#include "HailstormBullet.inl"