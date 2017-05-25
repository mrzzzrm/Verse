#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>

struct Voxel
{
    Voxel(const glm::uvec3 & cell):
        cell(cell)
    {
    }

    Voxel(const glm::uvec3 & cell, u32 colorIndex, float healthPoints):
        cell(cell),
        colorIndex(colorIndex),
        healthPoints(healthPoints)
    {}

    glm::uvec3  cell;
    u32         colorIndex;
    float       healthPoints;
};