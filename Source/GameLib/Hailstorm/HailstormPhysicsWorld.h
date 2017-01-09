#pragma once

#include <vector>

#include "HailstormBullet.h"

class VoxelWorld;

class HailstormPhysicsWorld final
{
public:
    HailstormPhysicsWorld(PhysicsWorld & physicsWorld, VoxelWorld & voxelWorld);

    void addBullet(const HailstormBullet & bullet);

    void update(float seconds);

private:
    VoxelWorld &                    m_voxelWorld;
    PhysicsWorld &                  m_physicsWorld;

    SparseVector<HailstormBullet>   m_bullets;
    std::vector<HailstormBulletID>  m_destroyedBullets;
};