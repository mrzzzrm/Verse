#include "PlayerSystem.h"

#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/ECS/Components.h>

#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Physics/PhysicsWorld.h>

#include <Deliberation/Platform/Input.h>

#include <Deliberation/Scene/Camera3D.h>

#include "AimHelper.h"
#include "Equipment.h"
#include "PlayerFlightControl.h"
#include "VoxelObject.h"

PlayerSystem::PlayerSystem(World & world,
                           InputBase & input,
                           Camera3D & camera,
                           PhysicsWorld & physicsWorld):
    Base(world, ComponentFilter::requires<
        RigidBodyComponent,
        VoxelObject,
        FlightControlConfig,
        PlayerFlightControl,
        Equipment>()),
    m_input(input),
    m_camera(camera),
    m_physicsWorld(physicsWorld),
    m_cameraDolly(m_camera)
{

}

void PlayerSystem::onEntityUpdate(Entity & entity, float seconds)
{
    auto & body = *entity.component<RigidBodyComponent>().value();
    auto & flightControlConfig = entity.component<FlightControlConfig>();
    auto & flightControl = entity.component<PlayerFlightControl>();

    glm::vec3 linearThrust;
    glm::vec3 angularThrust;

    if (m_input.keyDown(InputBase::Key_W)) linearThrust += glm::vec3(0.0f, 0.0f, -1.0f);
    if (m_input.keyDown(InputBase::Key_S)) linearThrust += glm::vec3(0.0f, 0.0f, 1.0f);
    if (m_input.keyDown(InputBase::Key_D)) linearThrust += glm::vec3(1.0f, 0.0f, 0.0f);
    if (m_input.keyDown(InputBase::Key_A)) linearThrust += glm::vec3(-1.0f, 0.0f, 0.0f);

    if (m_input.keyDown(InputBase::Key_Q)) angularThrust.z = 1;
    if (m_input.keyDown(InputBase::Key_E)) angularThrust.z = -1;

    flightControl.setLinearThrust(linearThrust);

    if (m_input.mouseButtonDown(InputBase::MouseButton_Left))
    {
        auto mouse = m_input.mousePosition();

        angularThrust.x = mouse.y;
        angularThrust.y = -mouse.x;

        flightControl.setAngularThrust(angularThrust);
    }
    else
    {
        flightControl.setAngularThrust(angularThrust);
    }

    {
        AimHelper aimHelper(m_camera, m_physicsWorld);

        auto & equipment = entity.component<Equipment>();

        bool hit;
        auto target = aimHelper.getTarget(m_input.mousePosition(), hit);

        if (m_input.mouseButtonDown(InputBase::MouseButton_Right))
        {
            equipment.setFireRequest(true, target);
        }
        else
        {
            equipment.setFireRequest(false, {});
        }
    }

    flightControl.update(body, flightControlConfig, seconds);
}

void PlayerSystem::onEntityPrePhysicsUpdate(Entity & entity, float seconds)
{
    auto & voxelObject = entity.component<VoxelObject>();
    auto & voxelData = voxelObject.data();

    auto & playerBody = *entity.component<RigidBodyComponent>().value();

    glm::vec3 offset;
    offset.z = voxelData.size().z * 1.4f;
    offset.y = voxelData.size().y * 2;

    Pose3D targetPose(playerBody.transform().position() +
                      playerBody.transform().orientation() * offset,
                      playerBody.transform().orientation());

    auto position = targetPose.pointLocalToWorld({});

    m_cameraDolly.update(position, targetPose.orientation(), seconds);
}