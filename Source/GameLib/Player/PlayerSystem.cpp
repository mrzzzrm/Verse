#include "PlayerSystem.h"

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
    m_camera(camera),
    m_physicsWorld(physicsWorld),
    m_cameraDolly(m_camera)
{
    auto program = m_context.createProgram({
        GameDataPath("Data/Shaders/UiElement.vert"),
        GameDataPath("Data/Shaders/UiElement.frag")});

    auto & resourceManager = world().system<ResourceManager>();
    auto mesh = resourceManager.mesh(R::UiCrosshairMesh);

    m_crosshairsDraw = m_context.createDraw(program);
    m_crosshairsDraw.setIndices(mesh.indices());
    m_crosshairsDraw.addVertices(mesh.vertices());
    m_crosshairsDraw.sampler("Texture").setTexture(mesh.textures()[0]);
    m_crosshairsDraw.state().setDepthState(DepthState::disabledRW());
    m_crosshairsDraw.state().setBlendState({gl::GL_FUNC_ADD, gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA});
    m_crosshairPositionUniform = m_crosshairsDraw.uniform("ElementPosition");
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

        if (m_leftMousePressedMillis != 0)
        {
            if (CurrentMillis() - m_leftMousePressedMillis < 300)
            {
                auto result = AimHelper(m_camera, m_physicsWorld).getTarget(m_input.mousePosition());

                if (result.hit)
                {
                    if (result.body->entity().isValid())
                    {
                        m_playerTarget = result.body->entity();
                    }
                }
            }
        }

        m_leftMousePressedMillis = 0;
    }

    if (m_input.mouseButtonPressed(InputBase::MouseButton_Left))
    {
        m_leftMousePressedMillis = CurrentMillis();
    }

    {
        AimHelper aimHelper(m_camera, m_physicsWorld);

        auto & equipment = entity.component<Equipment>();

        auto result = aimHelper.getTarget(m_input.mousePosition(),);

        if (m_input.mouseButtonDown(InputBase::MouseButton_Right))
        {
            equipment.setFireRequest(true, glm::normalize(result.pointOfImpact - body.transform().position()));
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

void PlayerSystem::renderUi()
{
    if (m_playerTarget.isValid())
    {
        const auto targetPosition = m_playerTarget.component<RigidBodyComponent>().value()->transform().position();

        const auto ray = Ray3D::fromTo(m_camera.position(), targetPosition);
        const auto nearPlane = m_camera.nearPlane();

        bool hit;
        auto nearPlanePosition = Rect3DRay3DIntersectionPoint(nearPlane, ray, hit);
        if (hit)
        {
            auto nearPlanePositionNS = nearPlanePosition / nearPlane.size();

            m_crosshairPositionUniform.set(nearPlanePositionWS);
            m_crosshairsDraw.schedule();
        }
    }
}