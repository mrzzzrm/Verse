#include <iostream>
#include <memory>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/Math/FloatUtils.h>
#include <Deliberation/Core/Math/Random.h>

#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/Systems/PhysicsWorldSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/Narrowphase.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/CameraDolly3D.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/Debug/DebugGeometryManager.h>
#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>
#include <Deliberation/Scene/SkyboxRenderer.h>

#include <Npc/NpcDebugRendererSystem.h>

#include "AimHelper.h"
#include "CollisionShapeTypes.h"
#include "Equipment.h"
#include "DebugOverlay.h"
#include "Emitter.h"
#include "PlayerFlightControl.h"
#include "NpcFlightControl.h"
#include "HailstormManager.h"
#include "Hardpoint.h"
#include "VfxManager.h"
#include "GameLib.h"
#include "NpcAttackTask.h"
#include "NpcController.h"
#include "NpcControllerSystem.h"
#include "NpcSteering.h"
#include "VoxelRigidBodyPayload.h"
#include "VoxelRenderChunkTree.h"
#include "VoxelWorld.h"
#include "VoxelClusterPrimitiveTest.h"
#include "VoxReader.h"
#include "VoxelRigidBodyPayload.h"
#include "VoxelClusterContact.h"
#include "Weapon.h"

class EntityPrototypeManager;

class VerseApplication:
    public Application
{
public:
    VerseApplication(const std::string & name);

    void onStartup() final override;
    void onFrame(float seconds) final override;

protected:
    virtual void onApplicationStartup() {};
    virtual void onApplicationUpdate(float seconds) {};
    virtual void onApplicationPhysicsUpdate(float physicsSeconds) {};
    virtual void onApplicationRender() {};

protected:
    Camera3D                m_camera;
    Clear                   m_clear;
    PhysicsWorld            m_physicsWorld;
    Optional<DebugGroundPlaneRenderer>
                            m_groundPlane;
    World                   m_world;

    std::shared_ptr<HailstormManager>
        m_hailstormManager;
    std::experimental::optional<VfxManager>
        m_vfxManager;

    std::experimental::optional<DebugGeometryManager>
        m_debugGeometryManager;

    std::shared_ptr<SkyboxRenderer>
        m_skyboxRenderer;
    std::shared_ptr<VoxelWorld>
        m_voxelWorld;

    std::shared_ptr<EntityPrototypeManager>
        m_entityPrototypeManager;
    std::shared_ptr<DebugOverlay>
        m_debugOverlay;
};
