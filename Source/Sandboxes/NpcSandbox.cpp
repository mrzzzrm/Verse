#include <iostream>
#include <memory>
#include <thread>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/Math/Random.h>

#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/Debug/DebugGeometryManager.h>
#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>

#include "Components.h"
#include "Emitter.h"
#include "Player/PlayerFlightControl.h"
#include "NpcFlightControl.h"
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

        FlightControlConfig flightControlConfig;
        flightControlConfig.forward.acceleration = 130.0f;
        flightControlConfig.forward.maxSpeed = 400.0f;
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
        m_rigidBody = std::make_shared<RigidBody>(voxelObject->data().shape());
        m_rigidBody->setPayload(rigidBodyPayload);
        m_rigidBody->transform().setCenter(glm::vec3(voxelObject->data().size()) / 2.0f);
        m_rigidBody->transform().setPosition(glm::vec3(0.0f, 50.0f, 0.0f));
       // m_rigidBody->transform().setOrientation();

        m_flightControl = std::make_shared<NpcFlightControl>(m_rigidBody, flightControlConfig);

        auto npcController = std::make_shared<NpcController>();
        m_task = std::make_shared<FlyToTask>(m_flightControl, glm::vec3{-63.370247,169.859543,179.064026});
        npcController->setTask(m_task);

        m_npc0.addComponent<VoxelObjectComponent>(voxelObject);
        m_npc0.addComponent<RigidBodyComponent>(m_rigidBody);
        m_npc0.addComponent<FlightControlComponent2>(m_flightControl);
        m_npc0.addComponent<NpcControllerComponent>(npcController);

        m_physicsWorld.addRigidBody(m_rigidBody);
        m_voxelWorld->addVoxelObject(voxelObject);

        m_debugGeometryManager.reset(context());
        m_debugGeometryRenderer.reset(*m_debugGeometryManager);
        m_debugGeometryRenderer->addArrow(m_rigidBody->transform().position(), {}, {0.8f, 0.8f, 0.8f});
        m_debugGeometryRenderer->addArrow(m_rigidBody->transform().position(), {}, {1.0f, 0.0f, 0.0f});
        m_debugGeometryRenderer->addArrow(m_rigidBody->transform().position(), {}, {0.0f, 1.0f, 0.0f});
    }

    void onFrame(float seconds) override
    {
        Pose3D pose;

        auto & body = m_npc0.component<RigidBodyComponent>().rigidBody;
        pose.setPosition(body->transform().position());
        pose.setOrientation(body->transform().orientation());
        pose.setCenter(body->transform().center());

        auto distance = glm::length(body->transform().position() -  m_task->destination());
        if (distance < 0.01f)
        {
            auto destination = RandomInHemisphere({0.0f, 300.0f, 0.0f});
            m_task->setDestination(destination);
            //m_rigidBody->setLinearVelocity(RandomInSphere() * 160.0f);
        }

        m_npc0.component<VoxelObjectComponent>().voxelObject->setPose(pose);
        m_npc0.component<NpcControllerComponent>().npcController->update(seconds);
        m_physicsWorld.update(seconds);
        m_npc0.component<FlightControlComponent2>().flightControl->update(seconds);

        m_debugGeometryRenderer->arrow(0).reset(m_rigidBody->transform().position(),
                                                m_task->destination() - m_rigidBody->transform().position());
        m_debugGeometryRenderer->arrow(1).reset(m_rigidBody->transform().position(),
                                                m_rigidBody->transform().directionLocalToWorld(m_flightControl->localLinearAcceleration()) * 10.0f);
        m_debugGeometryRenderer->arrow(2).reset(m_rigidBody->transform().position(),
                                                m_rigidBody->transform().directionLocalToWorld(m_flightControl->localAngularAccelertion()) * 100.0f);

        m_clear.schedule();
        m_groundPlane->schedule();
        m_navigator->update(seconds);
        m_voxelWorld->update(seconds);
        m_debugGeometryRenderer->schedule(m_camera);
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
    std::shared_ptr<FlyToTask>
                            m_task;
    std::shared_ptr<RigidBody>
                            m_rigidBody;
    std::shared_ptr<NpcFlightControl>
                            m_flightControl;

    Optional<DebugGeometryManager>
                            m_debugGeometryManager;
    Optional<DebugGeometryRenderer>
                            m_debugGeometryRenderer;
};

int main(int argc, char *argv[])
{
    return NpcSandbox().run(argc, argv);
}
