#pragma once

#include <glm/glm.hpp>

struct Voxel
{
    Voxel(const glm::uvec3 & cell, const glm::vec3 & color):
        cell(cell),
        color(color)
    {}

    glm::uvec3  cell;
    glm::vec3   color;
    bool        hull = false;
};