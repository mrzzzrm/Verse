#pragma once

#include <vector>

#include "HailstormParticle.h"

class VoxelWorld;

class HailstormPhysicsWorld final
{
public:
    HailstormPhysicsWorld(PhysicsWorld & physicsWorld, VoxelWorld & voxelWorld);

    /**
     * Bullets destroyed during the last update() call
     * @return
     */
    const std::vector<HailstormParticleID> & destroyedBullets() const;

    void addBullet(const HailstormParticle & bullet);

    void update(float seconds);

private:
    VoxelWorld &                    m_voxelWorld;
    PhysicsWorld &                  m_physicsWorld;

    SparseVector<HailstormParticle>   m_bullets;
    std::vector<HailstormParticleID>  m_destroyedBullets;
};