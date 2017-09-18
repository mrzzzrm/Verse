#pragma once

#include <memory>

#include <Deliberation/Core/Chrono.h>

#include <Deliberation/Draw/Draw.h>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Platform/InputManager.h>
#include <Deliberation/Platform/InputLayer.h>

#include <Deliberation/Scene/CameraDolly3D.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/Debug/DebugGeometryNode.h>

#include "GameLib.h"

namespace deliberation
{
class InputManager;
class Camera3D;
class PhysicsWorld;
class World;
} // namespace deliberation

class PlayerSystem : public System<PlayerSystem>,
                     public InputLayer
{
public:
    PlayerSystem(World & world);

    Entity &       player() { return m_player; }
    const Entity & player() const { return m_player; }
    Entity &       playerTarget() { return m_playerTarget; }
    const Entity & playerTarget() const { return m_playerTarget; }

    void setPlayerTarget(Entity & entity) { m_playerTarget = entity; }

    void onCreated() override { m_inputManager.addLayer(shared_from_this()); }
    void onRemoved() override { m_inputManager.removeLayer(shared_from_this()); }

    void onEntityAdded(Entity & entity) override;
    void onEntityRemoved(Entity & entity) override;
    void onEntityGameUpdate(Entity & entity, const UpdateFrame & updateFrame) override;
    void onGameUpdate(const UpdateFrame & updateFrame);
    void onFrameUpdate(const UpdateFrame & updateFrame);
    void onEntityPostPhysicsUpdate(Entity & entity, const UpdateFrame & updateFrame) override;
    void onFrameComplete(const UpdateFrame & updateFrame);

    void onMouseButtonDown(MouseStateEvent & event) override;
    void onMouseMotion(MouseMotionEvent & event) override;
    void onKeyPressed(KeyEvent & event) override;

private:
    enum class CameraMode
    {
        Normal,
        FreeFlight,
        Count
    };

private:
    InputManager &        m_inputManager;
    PhysicsWorld & m_physicsWorld;

    CameraMode             m_cameraMode;
    DebugCameraNavigator3D m_navigator;
    CameraDolly3D          m_cameraDolly;

    Entity m_player;
    Entity m_playerTarget;

    glm::vec3 m_linearThrust;
    glm::vec3 m_angularThrust;

    // DebugGeometryNode   m_debugGeometryRenderer;
};
