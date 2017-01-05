#pragma once

#include <vector>

#include "HailstormBullet.h"

class HailstormPhysicsWorld final
{
public:
    HailstormPhysicsWorld(PhysicsWorld & physicsWorld);

    void addBullet(const HailstormBullet & bullet);

    void update(float seconds);

private:
    PhysicsWorld &                  m_physicsWorld;

    std::vector<HailstormBullet>    m_bullets;
};