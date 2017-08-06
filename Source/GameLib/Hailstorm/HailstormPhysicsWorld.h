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
    const std::vector<HailstormBulletId> &    destroyedBullets() const;
    const std::vector<VoxelObjectBulletHit> & voxelObjectBulletHits() const;

    void addBullet(const HailstormBullet & bullet);

    void update(const UpdateFrame & updateFrame);

private:
    VoxelWorld &   m_voxelWorld;
    PhysicsWorld & m_physicsWorld;

    SparseVector<HailstormBullet>  m_bullets;
    /**
     * Separate lists, because destroyed bullets need to be removed from Vfx, for expired bullets this will happen
     * automatically (and removing a bullet twice shouldn't happen)
     */
    std::vector<HailstormBulletId> m_destroyedBullets;
    std::vector<HailstormBulletId> m_expiredBullets;

    std::vector<VoxelObjectBulletHit> m_voxelObjectBulletHits;
};