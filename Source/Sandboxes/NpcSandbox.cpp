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

#include "CollisionShapeTypes.h"
#include "Emitter.h"
#include "Player/PlayerFlightControl.h"
#include "NpcFlightControl.h"
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

using namespace deliberation;

class NpcSandbox:
    public Application
{
public:
    NpcSandbox():
        Application("NpcSandbox")
    {

    }

    void onStartup() override
    {
        //m_physicsWorld.narrowphase().registerPrimitiveTest((int)::CollisionShapeType::VoxelCluster, std::make_unique<VoxelClusterPrimitiveTest>());

        m_physicsWorld.narrowphase().contactDispatcher().
            registerContactType<VoxelClusterContact>((int)::CollisionShapeType::VoxelCluster);

        m_voxelWorld.reset(context(), m_physicsWorld, m_camera);

        m_camera.setPosition({0.0f, 200.0f, 300.0f});
        m_camera.setOrientation(glm::quat({-1.0f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / context().backbuffer().height());

        m_navigator.reset(m_camera, input(), 150.0f);

        m_clear = context().createClear();

        m_groundPlane.reset(context(), m_camera);
        m_groundPlane->setSize(1000.0f);
        m_groundPlane->setQuadSize(100.0f);
        m_groundPlane->setRadius(750.0f);

        VoxReader voxReader;
        {
            auto models = voxReader.read("Data/VoxelClusters/drone.vox");
            if (!models.empty())
            {
                m_voxelData = std::make_shared<VoxelObjectVoxelData>(*m_voxelWorld, models[0].size);
                m_voxelData->addVoxels(models[0].voxels);
            }
        }

        m_world.addSystem<PhysicsWorldSystem>(m_physicsWorld);
        m_world.addSystem<VoxelObjectSystem>(m_physicsWorld, *m_voxelWorld);
        m_world.addSystem<NpcControllerSystem>();

        auto npc0 = spawnNpc({-300.0f, 100.0f, 0.0f});
        auto npc1 = spawnNpc({0.0f, 150.0f, 0.0f});
        auto npc2 = spawnNpc({500.0f, 200.0f, 0.0f});

        {
            auto task = std::make_shared<NpcAttackTask>();
            task->setTarget(npc2);
            npc0.component<std::shared_ptr<NpcController>>()->setTask(task);
        }
        {
            auto task = std::make_shared<NpcAttackTask>();
            task->setTarget(npc0);
            npc2.component<std::shared_ptr<NpcController>>()->setTask(task);
        }
    }

    void onFrame(float seconds) override
    {
        m_world.update(seconds);

        auto physicsSimulationSeconds = m_physicsWorld.nextSimulationStep(seconds);
        if (EpsilonEq(physicsSimulationSeconds, 0.0f))
        {
            m_physicsWorld.update(seconds);
            return;
        }

        m_world.prePhysicsUpdate(physicsSimulationSeconds);
        m_physicsWorld.update(seconds);

        m_clear.schedule();
        m_groundPlane->schedule();
        m_navigator->update(physicsSimulationSeconds);
        m_voxelWorld->update(physicsSimulationSeconds);
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
        auto voxelObject = std::make_shared<VoxelObject>(*m_voxelData);

        auto rigidBodyPayload = std::make_shared<VoxelRigidBodyPayload>(voxelObject);
        auto rigidBody = std::make_shared<RigidBody>(voxelObject->data().shape());
        rigidBody->setPayload(rigidBodyPayload);
        rigidBody->transform().setCenter(glm::vec3(voxelObject->data().size()) / 2.0f);
        rigidBody->transform().setPosition(position);

        auto npcController = std::make_shared<NpcController>(flightControlConfig);

        npc.addComponent<std::shared_ptr<VoxelObject>>(voxelObject);
        npc.addComponent<std::shared_ptr<RigidBody>>(rigidBody);
        npc.addComponent<std::shared_ptr<NpcController>>(npcController);

        return npc;
    }

private:
    Camera3D                m_camera;
    Clear                   m_clear;
    PhysicsWorld            m_physicsWorld;
    Optional<DebugCameraNavigator3D>
                            m_navigator;
    Optional<VoxelWorld>    m_voxelWorld;
    Optional<DebugGroundPlaneRenderer>
                            m_groundPlane;
    World                   m_world;
    std::shared_ptr<VoxelObjectVoxelData>
                            m_voxelData;

};

int main(int argc, char *argv[])
{
    return NpcSandbox().run(argc, argv);
}
