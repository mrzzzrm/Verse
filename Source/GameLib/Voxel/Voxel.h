#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>

#include "GameLib.h"

struct Voxel
{
    Voxel() = default;

    Voxel(const glm::uvec3 & cell):
        cell(cell)
    {
    }

    Voxel(const glm::uvec3 & cell, u32 colorIndex, float healthPoints):
        cell(cell),
        colorIndex(colorIndex),
        healthPoints(healthPoints)
    {}

    Voxel(const Voxel & voxel) = default;

    glm::uvec3  cell;
    u32         colorIndex;
    float       healthPoints;
};