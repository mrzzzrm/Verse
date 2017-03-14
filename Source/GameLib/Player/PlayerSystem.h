#pragma once

#include <Deliberation/ECS/System.h>

#include <Deliberation/Scene/CameraDolly3D.h>

#include "GameLib.h"

namespace deliberation
{
class InputBase;
class Camera3D;
class PhysicsWorld;
class World;
}

class PlayerSystem:
    public System<PlayerSystem>
{
public:
    PlayerSystem(World & world, InputBase & input, Camera3D & camera, PhysicsWorld & physicsWorld);

protected:
    void onEntityUpdate(Entity & entity, float seconds) override;
    void onEntityPrePhysicsUpdate(Entity & entity, float seconds) override;

private:
    InputBase &     m_input;
    Camera3D &      m_camera;
    PhysicsWorld &  m_physicsWorld;

    CameraDolly3D   m_cameraDolly;
};
