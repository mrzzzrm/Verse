#include "PlayerSystem.h"

#include <Deliberation/Core/Math/Trajetory.h>
#include <Deliberation/Core/Math/PrimitiveIntersection.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Physics/PhysicsWorld.h>

#include <Deliberation/Platform/Input.h>

#include <Deliberation/Scene/Camera3D.h>

#include "AimHelper.h"
#include "Equipment.h"
#include "PlayerFlightControl.h"
#include "ResourceManager.h"
#include "VoxelObject.h"

PlayerSystem::PlayerSystem(World & world,
                           Context & context,
                           InputBase & input,
                           Camera3D & camera,
                           PhysicsWorld & physicsWorld):
    Base(world, ComponentFilter::requires<
        RigidBodyComponent,
        VoxelObject,
        FlightControlConfig,
        PlayerFlightControl,
        Equipment>()),
    m_context(context),
    m_input(input),
    m_cameraMode(CameraMode::Normal),
    m_camera(camera),
    m_navigator(m_camera, m_input, 150.0f),
    m_physicsWorld(physicsWorld),
    m_cameraDolly(m_camera)
{
    auto program = m_context.createProgram({
        GameDataPath("Data/Shaders/UiElement.vert"),
        GameDataPath("Data/Shaders/UiElement.frag")});

    auto & resourceManager = world.system<ResourceManager>();
    auto mesh = resourceManager.mesh(R::UiCrosshairMesh);

    m_crosshairsDraw = m_context.createDraw(program);
    m_crosshairsDraw.setIndices(mesh.indices());
    m_crosshairsDraw.addVertices(mesh.vertices());
    m_crosshairsDraw.sampler("Texture").setTexture(mesh.textures()[0]);
    m_crosshairsDraw.state().setDepthState(DepthState::disabledRW());
    m_crosshairsDraw.state().setBlendState({gl::GL_FUNC_ADD, gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA});
    m_crosshairPositionUniform = m_crosshairsDraw.uniform("ElementPosition");
    m_viewportSizeUniform = m_crosshairsDraw.uniform("ViewportSize");
}

void PlayerSystem::onEntityAdded(Entity & entity)
{
    Assert(!m_player.isValid(), "Can't have 2 player identities");
    m_player = entity;
}

void PlayerSystem::onEntityRemoved(Entity & entity)
{
    m_player = Entity();
}

void PlayerSystem::onEntityUpdate(Entity & entity, float seconds)
{
    auto &body = *entity.component<RigidBodyComponent>().value();
    auto &flightControlConfig = entity.component<FlightControlConfig>();
    auto &flightControl = entity.component<PlayerFlightControl>();

    if (m_cameraMode == CameraMode::Normal) {
        glm::vec3 linearThrust;
        glm::vec3 angularThrust;

        if (m_input.keyDown(InputBase::Key_W)) linearThrust += glm::vec3(0.0f, 0.0f, -1.0f);
        if (m_input.keyDown(InputBase::Key_S)) linearThrust += glm::vec3(0.0f, 0.0f, 1.0f);
        if (m_input.keyDown(InputBase::Key_D)) linearThrust += glm::vec3(1.0f, 0.0f, 0.0f);
        if (m_input.keyDown(InputBase::Key_A)) linearThrust += glm::vec3(-1.0f, 0.0f, 0.0f);

        if (m_input.keyDown(InputBase::Key_Q)) angularThrust.z = 1;
        if (m_input.keyDown(InputBase::Key_E)) angularThrust.z = -1;

        flightControl.setLinearThrust(linearThrust);

        if (m_input.mouseButtonDown(InputBase::MouseButton_Left)) {
            auto mouse = m_input.mousePosition();

            angularThrust.x = mouse.y;
            angularThrust.y = -mouse.x;

            flightControl.setAngularThrust(angularThrust);
        } else {
            flightControl.setAngularThrust(angularThrust);

            if (m_leftMousePressedMillis != 0) {
                auto result = AimHelper(m_camera, m_physicsWorld).getTarget(m_input.mousePosition());

                if (result.hit) {
                    if (result.body->entity().isValid()) {
                        m_playerTarget = result.body->entity();
                        std::cout << "Selected target: " << m_playerTarget.name() << std::endl;
                    }
                }
            }

            m_leftMousePressedMillis = 0;
        }

        if (m_input.mouseButtonPressed(InputBase::MouseButton_Left)) {
            m_leftMousePressedMillis = CurrentMillis();
        }

        {
            AimHelper aimHelper(m_camera, m_physicsWorld);

            auto &equipment = entity.component<Equipment>();

            auto result = aimHelper.getTarget(m_input.mousePosition());

            if (m_input.mouseButtonDown(InputBase::MouseButton_Right)) {
                equipment.setFireRequest(true, glm::normalize(result.pointOfImpact - body.transform().position()));
            } else {
                equipment.setFireRequest(false, {});
            }
        }
    }

    flightControl.update(body, flightControlConfig, seconds);

    if (m_input.keyPressed(InputBase::Key_C))
    {
        m_cameraMode = (CameraMode)(((int)m_cameraMode + 1) % (int)CameraMode::Count);
    }
}

void PlayerSystem::onEntityPrePhysicsUpdate(Entity & entity, float seconds)
{
    if (m_cameraMode == CameraMode::Normal)
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
    else if (m_cameraMode == CameraMode::FreeFlight)
    {
        m_navigator.update(seconds);
    }
}

void PlayerSystem::renderUi()
{
    if (!m_player.isValid()) return;

    if (m_playerTarget.isValid())
    {
        m_viewportSizeUniform.set(glm::vec2{m_context.backbuffer().width(), m_context.backbuffer().height()});

        const auto & targetBody = *m_playerTarget.component<RigidBodyComponent>().value();

        const auto targetPosition = targetBody.transform().position();
        const auto targetVelocity = targetBody.linearVelocity();

        const auto & body = *m_player.component<RigidBodyComponent>().value();
        const auto & equipment = m_player.component<Equipment>();

        const auto bulletSpeed = equipment.bulletSpeed();

        bool success;
        const auto trajectory = CalculateTrajectory(
            body.transform().position(), body.linearVelocity(),
            bulletSpeed, targetPosition, targetVelocity, success);

        const auto ray = Ray3D(m_camera.position(), trajectory);
        const auto nearPlane = m_camera.nearPlane();

        bool hit;
        auto nearPlanePosition = Rect3DRay3DIntersectionPoint(nearPlane, ray, hit);
        if (hit)
        {
            auto nearPlanePositionNS = nearPlanePosition * 2.0f  - 1.0f;

            m_crosshairPositionUniform.set(nearPlanePositionNS);
            m_crosshairsDraw.schedule();
        }
    }
}