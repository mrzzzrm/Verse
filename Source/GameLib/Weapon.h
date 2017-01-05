#pragma once

#include <glm/glm.hpp>

#include "Hailstorm/HailstormManager.h"

#include "GameLib.h"

struct WeaponConfig
{
    float                       frequency;
    HailstormRenderer::MeshID   meshID;
};

class Weapon final
{
public:
    Weapon(const WeaponConfig & config, HailstormManager & hailstormManager);

    void setFireRequest(bool active,
                        const glm::vec3 & origin = {},
                        const glm::vec3 & direction = {});

    void update(float seconds);

private:
    WeaponConfig        m_config;
    HailstormManager &  m_hailstormManager;

    bool                m_fireRequestActive = false;
    glm::vec3           m_fireRequestOrigin;
    glm::vec3           m_fireRequestDirection;

    float               m_cooldown = 0.0f;
};