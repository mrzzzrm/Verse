#pragma once

#include <vector>

#include <Deliberation/Core/SparseVector.h>

#include "HailstormBullet.h"
#include "VoxelImpactSystem.h"

namespace deliberation
{
class PhysicsWorld;
}

class VoxelWorld;

class HailstormPhysicsWorld final
{
public:
    HailstormPhysicsWorld(PhysicsWorld & physicsWorld, VoxelWorld & voxelWorld);

    /**
     * Bullets destroyed during the last update() call
     * @return
     */
    const std::vector<HailstormBulletId> & destroyedBullets() const;

    void addBullet(const HailstormBullet & bullet);

    void update(float seconds);

private:
    VoxelWorld &                    m_voxelWorld;
    PhysicsWorld &                  m_physicsWorld;

    VoxelImpactSystem               m_impactSystem;

    SparseVector<HailstormBullet>   m_bullets;
    std::vector<HailstormBulletId>  m_destroyedBullets;
};