#include "HudCrosshairs.h"

#include <Deliberation/Core/Math/Trajectory.h>
#include <Deliberation/Core/Math/PrimitiveIntersection.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Scene/Camera3D.h>

#include "Equipment.h"
#include "Hud.h"
#include "PlayerSystem.h"
#include "R.h"
#include "ResourceManager.h"
#include "VoxelObject.h"
#include "Weapon.h"

HudCrosshairs::HudCrosshairs(Hud & hud):
    HudLayer(hud),
    m_context(hud.world().system<ApplicationSystem>().context()),
    m_playerSystem(hud.world().system<PlayerSystem>()) 
{
    auto & resourceManager = hud.world().system<ResourceManager>();
    auto mesh = resourceManager.mesh(R::UiCrosshairMesh);
    auto program = resourceManager.program(R::HudElement);

    m_draw = m_context.createDraw(program);
    m_draw.setIndices(mesh.indices());
    m_draw.addVertices(mesh.vertices());
    m_draw.sampler("Texture").setTexture(mesh.textures()[0]);
    m_draw.setAttribute("Flip", glm::vec2(1.0f));
    m_draw.setAttribute("ElementColor", glm::vec3(0.5f, 0.4f, 1.0f));
    m_draw.state().setDepthState(DepthState::disabledR());
    m_draw.state().setBlendState({gl::GL_FUNC_ADD, gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA});
    m_viewportSizeUniform = m_draw.uniform("ViewportSize");
}

void HudCrosshairs::update(float seconds) 
{
    const auto halfExtent = glm::vec2(32.0f) / glm::vec2(m_context.backbuffer().size());
    setHalfExtent(halfExtent);

    auto & player = m_playerSystem.player();
    auto & playerTarget = m_playerSystem.playerTarget();

    if (player.isValid() && playerTarget.isValid())
    {
        const auto & targetBody = *playerTarget.component<RigidBodyComponent>().value();

        const auto targetPosition = targetBody.transform().position();
        const auto targetVelocity = targetBody.linearVelocity();

        const auto & body = *player.component<RigidBodyComponent>().value();
        const auto & equipment = player.component<Equipment>();

        const auto bulletSpeed = equipment.bulletSpeed();

        bool success;
        m_trajectory = CalculateTrajectory(
            body.transform().position(), body.linearVelocity(),
            bulletSpeed, targetPosition, targetVelocity, success);

        const auto ray = Ray3D(m_hud.camera().position(), m_trajectory);
        const auto nearPlane = m_hud.camera().nearPlane();

        bool hit;
        auto nearPlanePosition = Rect3DRay3DIntersectionPoint(nearPlane, ray, hit);
        if (hit)
        {
            setPosition(nearPlanePosition * 2.0f  - 1.0f);
        }

        setVisible(hit);
    }
    else
    {
        setVisible(false);
    }
}

void HudCrosshairs::render() 
{
    if (!visible()) return;

    auto & player = m_playerSystem.player();
    auto & playerTarget = m_playerSystem.playerTarget();

    if (player.isValid() && playerTarget.isValid())
    {
        m_viewportSizeUniform.set(glm::vec2{m_context.backbuffer().size()});

        if (visible())
        {
            m_draw.setAttribute("ElementPosition", position());
            m_draw.schedule();
        }
    }  
}

void HudCrosshairs::onMouseButtonDown(MouseButtonEvent & event)
{
    if (event.button() != MouseButton_Right) return;

    auto & player = m_playerSystem.player();
    auto & playerTarget = m_playerSystem.playerTarget();

    if (!playerTarget.isValid() || !player.isValid()) return;

    const auto & targetBody = *playerTarget.component<RigidBodyComponent>().value();

    auto & equipment = player.component<Equipment>();
    const auto & body = *player.component<RigidBodyComponent>().value();
    const auto & voxelObject = player.component<VoxelObject>();

    const auto & targetPosition = targetBody.transform().position();
    const auto & targetVelocity = targetBody.linearVelocity();

    Transform3D equipmentTransform = body.transform();
    equipmentTransform.setScale(voxelObject.scale());

    equipment.setFireRequestTargetForAllHardpoints(
        equipmentTransform,
        body.linearVelocity(),
        targetPosition,
        targetVelocity);

    event.consume();
}