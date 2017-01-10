#pragma once

#include <glm/glm.hpp>

#include "GameLib.h"
#include "HailstormManager.h"
#include "VoxelDefines.h"

struct WeaponConfig
{
    float             cooldown = 0.0f;
    HailstormMeshID   meshID = 0;
};

class Weapon final
{
public:
    Weapon(const WeaponConfig & config, HailstormManager & hailstormManager, VoxelObjectWorldUID creatorUID);

    void setFireRequest(bool active,
                        const glm::vec3 & origin = {},
                        const glm::vec3 & direction = {});

    void update(float seconds);

private:
    WeaponConfig        m_config;
    HailstormManager &  m_hailstormManager;
    VoxelObjectWorldUID m_creatorUID;

    bool                m_fireRequestActive = false;
    glm::vec3           m_fireRequestOrigin;
    glm::vec3           m_fireRequestDirection;

    float               m_cooldown = 0.0f;
};