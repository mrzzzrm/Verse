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
#include <Systems/VoxelObjectSystem.h>
#include <Npc/NpcDebugRendererSystem.h>

#include "AimHelper.h"
#include "CollisionShapeTypes.h"
#include "Equipment.h"
#include "Emitter.h"
#include "PlayerFlightControl.h"
#include "NpcFlightControl.h"
#include "Hardpoint.h"
#include "HailstormManager.h"
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
#include "VoxelObjectPrimitives.h"
#include "Weapon.h"

#include "SandboxApplication.h"

using namespace deliberation;

class ShootingRangeSandbox:
    public SandboxApplication
{
public:
    ShootingRangeSandbox():
        SandboxApplication("ShootingRangeSandbox")
    {

    }

    void onSandboxStartup() override
    {
        {
            auto data = BuildVoxelBlock(*m_voxelWorld, {25, 3, 3}, {0.8f, 0.9f, 0.0f});
            auto entity = m_world.createEntity("Block");

            auto voxelObject = std::make_shared<VoxelObject>(data);

            auto rigidBodyPayload = std::make_shared<VoxelRigidBodyPayload>(voxelObject);
            auto rigidBody = std::make_shared<RigidBody>(voxelObject->data().shape());
            rigidBody->setPayload(rigidBodyPayload);
            rigidBody->transform().setCenter(glm::vec3(voxelObject->data().size()) / 2.0f);
            rigidBody->transform().setPosition({0.0f, 30.0f, 0.0f});

            entity.addComponent<std::shared_ptr<VoxelObject>>(voxelObject);
            entity.addComponent<std::shared_ptr<RigidBody>>(rigidBody);
        }

        auto bulletMesh = UVSphere(5, 5).generateMesh2();
        m_bulletMeshID = m_hailstormManager->renderer().addMesh(bulletMesh);

        WeaponConfig weaponConfig;
        weaponConfig.cooldown = 0.1f;
        weaponConfig.meshID = m_bulletMeshID;
        m_hardpoint = std::make_shared<Hardpoint>(Pose3D(), glm::pi<float>());
        m_hardpoint->setWeapon(std::make_shared<Weapon>(weaponConfig,
                                                        *m_hailstormManager,
                                                        INVALID_VOXEL_OBJECT_WORLD_UID));
    }

    void onSandboxUpdate(float seconds) override
    {
        if (input().mouseButtonDown(InputBase::MouseButton_Right))
        {
            AimHelper aimHelper(m_camera, m_physicsWorld);
            auto hit = false;
            auto target = aimHelper.getTarget(input().mousePosition(), hit);

            m_hardpoint->setFireRequest(true, target);
        }
        else
        {
            m_hardpoint->setFireRequest(false, {});
        }

        EquipmentUpdateContext context;
        context.targetPose = m_camera.pose().localTranslated({5.0f, 0.0f, -15.0f});

        m_hardpoint->update(seconds, context);
    }

    void onSandboxPhysicsUpdate(float physicsSeconds) override
    {

    }

    void onSandboxRender() override
    {

    }

private:
    std::shared_ptr<Hardpoint> m_hardpoint;
    HailstormMeshID             m_bulletMeshID;
};

int main(int argc, char *argv[])
{
    return ShootingRangeSandbox().run(argc, argv);
}
