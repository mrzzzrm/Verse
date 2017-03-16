#pragma once

#include <Deliberation/Core/Chrono.h>

#include <Deliberation/Draw/Draw.h>

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
    PlayerSystem(World & world, Context & context, InputBase & input, Camera3D & camera, PhysicsWorld & physicsWorld);

    void renderUi();

protected:
    void onEntityUpdate(Entity & entity, float seconds) override;
    void onEntityPrePhysicsUpdate(Entity & entity, float seconds) override;

private:
    Context &       m_context
    InputBase &     m_input;
    Camera3D &      m_camera;
    PhysicsWorld &  m_physicsWorld;

    CameraDolly3D   m_cameraDolly;

    TimestampMillis m_leftMousePressedMillis = 0;

    Entity          m_playerTarget;

    Draw            m_crosshairsDraw;
    Uniform         m_crosshairPositionUniform;
};
