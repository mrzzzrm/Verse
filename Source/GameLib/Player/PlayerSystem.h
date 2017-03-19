#pragma once

#include <Deliberation/Core/Chrono.h>

#include <Deliberation/Draw/Draw.h>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Scene/CameraDolly3D.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>

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
    PlayerSystem(World & world,
                 Context & context,
                 InputBase & input,
                 Camera3D & camera,
                 PhysicsWorld & physicsWorld);

    void renderUi();

protected:
    void onEntityAdded(Entity & entity) override;
    void onEntityRemoved(Entity & entity) override;
    void onEntityUpdate(Entity & entity, float seconds) override;
    void onEntityPrePhysicsUpdate(Entity & entity, float seconds) override;

private:
    enum class CameraMode
    {
        Normal,
        FreeFlight,
        Count
    };

private:
    Context &               m_context;
    InputBase &             m_input;
    Camera3D &              m_camera;
    PhysicsWorld &          m_physicsWorld;

    CameraMode              m_cameraMode;
    DebugCameraNavigator3D  m_navigator;
    CameraDolly3D           m_cameraDolly;

    TimestampMillis         m_leftMousePressedMillis = 0;

    Entity                  m_player;
    Entity                  m_playerTarget;

    Draw                    m_crosshairsDraw;
    Uniform                 m_viewportSizeUniform;
    Uniform                 m_crosshairPositionUniform;
};
