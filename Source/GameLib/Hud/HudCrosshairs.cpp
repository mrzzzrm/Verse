#include "HudCrosshairs.h"

#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/Math/PrimitiveIntersection.h>
#include <Deliberation/Core/Math/Trajectory.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/ECS/RigidBodyComponent.h>
#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/Transform3DComponent.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Resource/ResourceManager.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>

#include <Deliberation/Platform/App.h>
#include <Deliberation/Platform/AppRuntime.h>

#include "Equipment.h"
#include "Hud.h"
#include "PlayerSystem.h"
#include "VoxelObject.h"
#include "Weapon.h"

HudCrosshairs::HudCrosshairs(Hud & hud)
    : HudLayer(hud)
    , m_playerSystem(hud.world().systemRef<PlayerSystem>())
{
    auto & resourceManager = *GetGlobal<ResourceManager>();
    auto   mesh = resourceManager.mesh(R::UiCrosshairMesh);
    auto   program = resourceManager.program(R::HudElement);

    m_draw = GetGlobal<DrawContext>()->createDraw(program);
    m_draw.setIndices(mesh.indices());
    m_draw.addVertices(mesh.vertices());
    m_draw.sampler("Texture").setTexture(mesh.textures()[0]);
    m_draw.setAttribute("Flip", glm::vec2(1.0f));
    m_draw.setAttribute("ElementColor", glm::vec3(0.5f, 0.4f, 1.0f));
    m_draw.state().setDepthState(DepthState::disabledR());
    m_draw.state().setBlendState({BlendEquation::Add,
                                  BlendFactor::SourceAlpha,
                                  BlendFactor::OneMinusSourceAlpha});
    m_viewportSizeUniform = m_draw.uniform("ViewportSize");
}

void HudCrosshairs::update(const UpdateFrame & updateFrame)
{
    const auto halfExtent =
        glm::vec2(32.0f) / glm::vec2(GetGlobal<DrawContext>()->backbuffer().size());
    setHalfExtent(halfExtent);

    auto & player = m_playerSystem.player();
    auto & playerTarget = m_playerSystem.playerTarget();

    if (player.isValid() && playerTarget.isValid())
    {
        const auto & targetBody =
            *playerTarget.component<RigidBodyComponent>().value();

        const auto targetPosition = targetBody.transform().position();
        const auto targetVelocity = targetBody.linearVelocity();

        const auto & body = *player.component<RigidBodyComponent>().value();
        const auto & equipment = player.component<Equipment>();

        const auto bulletSpeed = equipment.bulletSpeed();

        bool success;
        m_trajectory = CalculateTrajectory(
            body.transform().position(),
            body.linearVelocity(),
            bulletSpeed,
            targetPosition,
            targetVelocity,
            success);

        if (success) {
            const auto ray =
                Ray3D(GetGlobal<RenderManager>()->mainCamera().position(), m_trajectory);
            const auto nearPlane = GetGlobal<RenderManager>()->mainCamera().nearPlane();

            bool hit;
            auto nearPlanePosition =
                Rect3DRay3DIntersectionPoint(nearPlane, ray, hit);
            if (hit) {
                setPosition(nearPlanePosition * 2.0f - 1.0f);
            }

            setVisible(hit);
        } else
        {
            setVisible(false);
        }
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
        m_viewportSizeUniform.set(glm::vec2{GetGlobal<DrawContext>()->backbuffer().size()});

        if (visible())
        {
            m_draw.setAttribute("ElementPosition", position());
            m_draw.render();
        }
    }
}

void HudCrosshairs::onMouseButtonPressed(MouseButtonEvent & event)
{
    if (event.button() != MouseButton::Right) return;

    auto & player = m_playerSystem.player();
    auto & playerTarget = m_playerSystem.playerTarget();

    if (!playerTarget.isValid() || !player.isValid()) return;

    const auto & targetBody =
        *playerTarget.component<RigidBodyComponent>().value();

    auto &       equipment = player.component<Equipment>();
    const auto & body = *player.component<RigidBodyComponent>().value();

    const auto & targetPosition = targetBody.transform().position();
    const auto & targetVelocity = targetBody.linearVelocity();

    const auto & equipmentTransform =
        player.component<Transform3DComponent>().transform();

    equipment.setFireRequestTargetForAllHardpoints(
        equipmentTransform,
        body.linearVelocity(),
        targetPosition,
        targetVelocity);

    event.consume();
}