#include <iostream>
#include <memory>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <Deliberation/Core/Math/FloatUtils.h>
#include <Deliberation/Core/Math/Random.h>
#include <Deliberation/Core/Optional.h>

#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/Systems/PhysicsWorldSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/Narrowphase.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/CameraDolly3D.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/Debug/DebugGeometryNode.h>
#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>
#include <Deliberation/Scene/SkyboxRenderer.h>
#include <Deliberation/Scene/UVSphere.h>

#include <Npc/NpcDebugRendererSystem.h>

#include "AimHelper.h"
#include "CollisionShapeTypes.h"
#include "DebugOverlay.h"
#include "Emitter.h"
#include "Equipment.h"
#include "GameLib.h"
#include "HailstormManager.h"
#include "Hardpoint.h"
#include "NpcAttackTask.h"
#include "NpcController.h"
#include "NpcControllerSystem.h"
#include "NpcFlightControl.h"
#include "NpcSteering.h"
#include "PlayerFlightControl.h"
#include "VfxManager.h"
#include "VoxReader.h"
#include "VoxelClusterContact.h"
#include "VoxelClusterPrimitiveTest.h"
#include "VoxelRenderable.h"
#include "VoxelRigidBodyPayload.h"
#include "VoxelWorld.h"
#include "Weapon.h"

namespace deliberation
{
class EntityPrototypeManager;
class PhysicsWorldSystem;
}

enum class VerseApplicationSystemInitMode
{
    NoSystems,
    AllSystems
};

class VerseApplication : public Application
{
public:
    VerseApplication(
        const std::string &            name,
        VerseApplicationSystemInitMode systemInitMode =
            VerseApplicationSystemInitMode::AllSystems);

    void onStartup() final override;
    void onFrame(DurationMicros micros) final override;

protected:
    virtual void onApplicationStartup(){};
    virtual void onApplicationUpdate(){};
    virtual void onApplicationPhysicsUpdate(){};

protected:
    VerseApplicationSystemInitMode m_systemInitMode;

    PhysicsWorld m_physicsWorld;
    World        m_world;

    Texture m_skyboxCubemap;

    std::shared_ptr<PhysicsWorldSystem> m_physicsWorldSystem;
};
