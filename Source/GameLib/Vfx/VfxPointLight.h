#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>

#include "GameLib.h"

struct VfxPointLightDesc
{
    float intensity = 1.0f;
};

struct VfxPointLight
{
    VfxPointLightDesc desc;
    glm::vec3 velocity;
    glm::vec3 birthRgb;
    glm::vec3 deathRgb;
    size_t pointLight = INVALID_SIZE_T;
    size_t particleUID = 0;
};