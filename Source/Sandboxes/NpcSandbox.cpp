#include <iostream>
#include <memory>
#include <thread>

#include <json.hpp>

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
#include "Components.h"
#include "CollisionShapeTypes.h"
#include "Equipment.h"
#include "EquipmentPrototype.h"
#include "Emitter.h"
#include "PlayerFlightControl.h"
#include "NpcFlightControl.h"
#include "Hardpoint.h"
#include "VfxManager.h"
#include "NpcAttackTask.h"
#include "NpcController.h"
#include "NpcControllerSystem.h"
#include "NpcDebugTask.h"
#include "NpcSteering.h"
#include "VoxelRigidBodyPayload.h"
#include "VoxelRenderChunkTree.h"
#include "VoxelWorld.h"
#include "VoxelClusterPrimitiveTest.h"
#include "VoxReader.h"
#include "VoxelRigidBodyPayload.h"
#include "VoxelClusterContact.h"
#include "Weapon.h"

#include "SandboxApplication.h"

using namespace deliberation;

class NpcSandbox:
    public SandboxApplication
{
public:
    NpcSandbox():
        SandboxApplication("NpcSandbox")
    {

    }

    void onSandboxStartup() override
    {
        {
            std::ifstream equipmentPrototypeFile("Data/Prototypes/Ship.json");

            nlohmann::json obj;
            equipmentPrototypeFile >> obj;

            m_npcEquipmentPrototype = std::make_shared<EquipmentPrototype>(obj["Equipment"]);
        }


        {
            auto lifetime = std::make_shared<EmitterRandomLifetime>(0.9f, 1.2f);
            auto placement = std::make_shared<EmitterFixedPlacement>();
            auto velocity = std::make_shared<EmitterFixedDirection>(80.0f, 89.0f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(120, 0.0f);
            auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
            auto color = std::make_shared<EmitterColorOverLifetime>(glm::vec4{0.4f, 0.4f, 0.9f, 0.6f},
                                                                    glm::vec4{0.8f, 0.8f, 0.0f, 0.5f});
            auto size = std::make_shared<EmitterSizeOverLifetime>(7.0f, 1.0f);

            m_emitterAfterburner = std::make_shared<Emitter>(
                *m_vfxManager,
                m_vfxManager->baseParticleMeshId(),
                velocity,
                rotation,
                placement,
                intensity,
                lifetime,
                color,
                size);
        }

        {
            auto lifetime = std::make_shared<EmitterRandomLifetime>(2.5f, 5.0f);
            auto placement = std::make_shared<EmitterGaussianSphericalPlacement>(3.0f, 1.0f);
            auto velocity = std::make_shared<EmitterFixedDirection>(40.0f, 89.0f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(50, 0.0f);
            auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
            auto color = std::make_shared<EmitterColorOverLifetime>(glm::vec4{0.4f, 0.4f, 0.9f, 0.2f},
                                                                    glm::vec4{0.4f, 0.4f, 0.9f, 0.0f});
            auto size = std::make_shared<EmitterSizeOverLifetime>(16.0f, 15.0f);

            auto emitter = std::make_shared<Emitter>(
                *m_vfxManager,
                m_vfxManager->baseParticleMeshId(),
                velocity,
                rotation,
                placement,
                intensity,
                lifetime,
                color,
                size);

            m_emitterAfterburner->addChild(emitter);
        }

        {
            auto lifetime = std::make_shared<EmitterRandomLifetime>(0.9f, 1.2f);
            auto placement = std::make_shared<EmitterFixedPlacement>();
            auto velocity = std::make_shared<EmitterConeStrategy>(0.1f, 280.0f, 289.0f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(7, 0.0f);
            auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
            auto color = std::make_shared<EmitterColorOverLifetime>(glm::vec4{1.0f, 0.8f, 0.0f, 0.8f},
                                                                    glm::vec4{0.8f, 0.8f, 0.0f, 0.5f});
            auto size = std::make_shared<EmitterSizeOverLifetime>(2.5f, 2.0f);

            auto emitter = std::make_shared<Emitter>(
                *m_vfxManager,
                m_vfxManager->baseParticleMeshId(),
                velocity,
                rotation,
                placement,
                intensity,
                lifetime,
                color,
                size);

            m_emitterAfterburner->addChild(emitter);
        }

        VoxReader voxReader;
        {
            auto models = voxReader.read("Data/VoxelClusters/ship.vox");
            if (!models.empty())
            {
                m_voxelData = std::make_shared<VoxelObjectVoxelData>(*m_voxelWorld, models[0].size);
                m_voxelData->addVoxels(models[0].voxels);
            }
        }

        auto bulletMesh = UVSphere(5, 5).generateMesh2();
        m_bulletMeshID = m_hailstormManager->vfxManager().renderer().addMesh(bulletMesh);

        auto npc0 = spawnNpc({-300.0f, 400.0f, 0.0f});
        auto npc1 = spawnNpc({0.0f, 150.0f, 0.0f});
        auto npc2 = spawnNpc({500.0f, 700.0f, 0.0f});

        {
            auto task = std::make_shared<NpcAttackTask>();
            task->setTarget(npc2);
            npc0.component<NpcController>().setTask(task);
        }
        {
            auto task = std::make_shared<NpcAttackTask>();
            task->setTarget(npc0);
            npc2.component<NpcController>().setTask(task);
        }
        {
            auto task = std::make_shared<NpcDebugTask>();
            task->setFireRequest(true, glm::vec3(40.0f, 150.0f, -200.0f));
            npc1.component<NpcController>().setTask(task);
        }


        WeaponConfig weaponConfig;
        weaponConfig.cooldown = 0.1f;
        weaponConfig.meshID = m_bulletMeshID;
        m_hardpoint = std::make_shared<Hardpoint>(glm::uvec3(), Pose3D(), glm::pi<float>());
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

    void onSandboxRender() override
    {
    }

    Entity spawnNpc(const glm::vec3 & position)
    {
        FlightControlConfig flightControlConfig;
        flightControlConfig.forward.acceleration = 130.0f;
        flightControlConfig.forward.maxSpeed = 100.0f;
        flightControlConfig.backward.acceleration = 20.0f;
        flightControlConfig.backward.maxSpeed = 60.0f;
        flightControlConfig.horizontal.acceleration = 20.0f;
        flightControlConfig.horizontal.maxSpeed = 60.0f;
        flightControlConfig.vertical.acceleration = 20.0f;
        flightControlConfig.vertical.maxSpeed = 60.0f;
        flightControlConfig.angular.acceleration = 3.0f;
        flightControlConfig.angular.maxSpeed = 2.0f;

        auto npc = m_world.createEntity("npc");
        auto & voxelObject = npc.addComponent<VoxelObject>(*m_voxelData);

        auto rigidBodyPayload = std::make_shared<VoxelRigidBodyPayload>(voxelObject.shared_from_this());

        auto rigidBody = std::make_shared<RigidBody>(voxelObject.data().shape());
        rigidBody->setPayload(rigidBodyPayload);
        rigidBody->transform().setCenter(glm::vec3(voxelObject.data().size()) / 2.0f);
        rigidBody->transform().setPosition(position);

        auto & npcController = npc.addComponent<NpcController>(flightControlConfig);

        npc.addComponent<RigidBodyComponent>(rigidBody);

        m_npcEquipmentPrototype->applyToEntity(npc, *m_vfxManager);

        auto & equipment = npc.component<Equipment>();

        equipment.setEngine(0, std::make_shared<Engine>(m_emitterAfterburner));
        equipment.setEngine(1, std::make_shared<Engine>(m_emitterAfterburner));
//        equipment.setEngine(2, std::make_shared<Engine>(m_emitterAfterburner));
//        equipment.setEngine(3, std::make_shared<Engine>(m_emitterAfterburner));

        {
            WeaponConfig weaponConfig;
            weaponConfig.cooldown = 0.2f;
            weaponConfig.meshID = m_bulletMeshID;

            auto weapon0 = std::make_shared<Weapon>(weaponConfig, *m_hailstormManager, voxelObject.id().worldUID);
            auto weapon1 = std::make_shared<Weapon>(weaponConfig, *m_hailstormManager, voxelObject.id().worldUID);

            equipment.setWeapon(0, weapon0);
            equipment.setWeapon(1, weapon1);
        }


        return npc;
    }

private:
    std::shared_ptr<EquipmentPrototype> m_npcEquipmentPrototype;

    std::shared_ptr<Hardpoint>  m_hardpoint;

    std::shared_ptr<VoxelObjectVoxelData>
                                m_voxelData;

    VfxMeshId                   m_bulletMeshID = -1;
    std::shared_ptr<Emitter>    m_emitterAfterburner;

};

int main(int argc, char *argv[])
{
    return NpcSandbox().run(argc, argv);
}
