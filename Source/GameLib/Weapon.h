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
    Weapon(const WeaponConfig & config,
           HailstormManager & hailstormManager,
           VoxelObjectWorldUID creatorUID);

    void setFireRequest(bool active,
                        const glm::vec3 & target);

    void setPosition(const glm::vec3 & position);

    void update(float seconds, const glm::vec3 & position);

private:
    WeaponConfig        m_config;
    HailstormManager &  m_hailstormManager;
    VoxelObjectWorldUID m_creatorUID;

    glm::vec3           m_position;

    bool                m_fireRequestActive = false;
    glm::vec3           m_fireRequestTarget;

    float               m_cooldown = 0.0f;
};