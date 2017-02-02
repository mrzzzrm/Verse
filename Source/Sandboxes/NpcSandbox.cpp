#include <iostream>
#include <memory>
#include <thread>

#include <Deliberation/Core/Optional.h>

#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>

#include "Components.h"
#include "Emitter.h"
#include "FlightControl.h"
#include "HailstormManager.h"
#include "NpcController.h"
#include "FlyToTask.h"
#include "VoxelRenderChunkTree.h"
#include "VoxelWorld.h"
#include "VoxelClusterPrimitiveTest.h"
#include "VoxReader.h"
#include "VoxelRigidBodyPayload.h"

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
        m_voxelWorld.reset(context(), m_physicsWorld, m_camera);

        m_camera.setPosition({0.0f, 400.0f, 500.0f});
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

        FlightControlConfig flightControlConfig;
        flightControlConfig.forward.acceleration = 30.0f;
        flightControlConfig.forward.maxSpeed = 100.0f;
        flightControlConfig.backward.acceleration = 20.0f;
        flightControlConfig.backward.maxSpeed = 60.0f;
        flightControlConfig.horizontal.acceleration = 20.0f;
        flightControlConfig.horizontal.maxSpeed = 60.0f;
        flightControlConfig.vertical.acceleration = 20.0f;
        flightControlConfig.vertical.maxSpeed = 60.0f;
        flightControlConfig.angular.acceleration = 3.0f;
        flightControlConfig.angular.maxSpeed = 2.0f;

        m_npc0 = m_world.createEntity("npc0");
        auto voxelObject = std::make_shared<VoxelObject>(*m_voxelData);

        auto rigidBodyPayload = std::make_shared<VoxelRigidBodyPayload>(voxelObject);
        auto rigidBody = std::make_shared<RigidBody>(voxelObject->data().shape());
        rigidBody->setPayload(rigidBodyPayload);
        rigidBody->transform().setCenter(glm::vec3(voxelObject->data().size()) / 2.0f);

        auto flightControl = std::make_shared<FlightControl>(rigidBody, flightControlConfig);

        auto npcController = std::make_shared<NpcController>();
        npcController->setTask(std::make_shared<FlyToTask>(flightControl, glm::vec3(100, 200, -50)));

        m_npc0.addComponent<VoxelObjectComponent>(voxelObject);
        m_npc0.addComponent<RigidBodyComponent>(rigidBody);
        m_npc0.addComponent<FlightControlComponent2>(flightControl);
        m_npc0.addComponent<NpcControllerComponent>(npcController);

        m_physicsWorld.addRigidBody(rigidBody);
        m_voxelWorld->addVoxelObject(voxelObject);
    }

    void onFrame(float seconds) override
    {
        Pose3D pose;

        auto & body = m_npc0.component<RigidBodyComponent>().rigidBody;
        pose.setPosition(body->transform().position());
        pose.setOrientation(body->transform().orientation());
        pose.setCenter(body->transform().center());

        m_npc0.component<VoxelObjectComponent>().voxelObject->setPose(pose);
        m_npc0.component<NpcControllerComponent>().npcController->update(seconds);

        m_clear.schedule();
        m_groundPlane->schedule();
        m_physicsWorld.update(seconds);
        m_navigator->update(seconds);
        m_voxelWorld->update(seconds);
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
    Entity                  m_npc0;
};

int main(int argc, char *argv[])
{
    return NpcSandbox().run(argc, argv);
}
