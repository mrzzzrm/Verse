#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/ECS/Entity.h>

#include "GameLib.h"
#include "HailstormBullet.h"

class VoxelObject;

struct VoxelObjectBulletHit
{
    VoxelObjectBulletHit(const Entity & entity, const glm::uvec3 & voxel, const HailstormBullet & bullet)
        : entity(entity), voxel(voxel), bullet(bullet)
    {
    }

    Entity          entity;
    glm::uvec3      voxel;
    HailstormBullet bullet;
};