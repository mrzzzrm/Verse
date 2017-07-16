#pragma once

#include <memory>

#include <Deliberation/ECS/Entity.h>

#include "GameLib.h"

class VoxelObject;

struct VoxelObjectBulletHit
{
    VoxelObjectBulletHit(const Entity & entity, const glm::uvec3 & voxel):
        entity(entity), voxel(voxel)
    {}

    Entity      entity;
    glm::uvec3  voxel;
    float       damage = 0.0f;
};