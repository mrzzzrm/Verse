#pragma once

#include <vector>

#include <Deliberation/Core/SparseVector.h>

#include "HailstormBullet.h"
#include "VoxelImpactSystem.h"
#include "VoxelObjectBulletHit.h"
#include "VoxelObjectModification.h"

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
    const std::vector<VoxelObjectModification> & voxelObjectModifications() const;
    const std::vector<VoxelObjectBulletHit> & voxelObjectBulletHits() const;

    void addBullet(const HailstormBullet & bullet);

    void update(float seconds);

private:
    VoxelWorld &                            m_voxelWorld;
    PhysicsWorld &                          m_physicsWorld;

    SparseVector<HailstormBullet>           m_bullets;
    std::vector<HailstormBulletId>          m_destroyedBullets;

    std::vector<VoxelObjectModification>    m_voxelObjectModifications;
    std::vector<VoxelObjectBulletHit>       m_voxelObjectBulletHits;
};