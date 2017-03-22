#include "HudEntityMarkers.h"

#include <Deliberation/Draw/Context.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>

#include <Deliberation/Scene/Camera3D.h>

#include "Hud.h"
#include "HudButton.h"
#include "PlayerFlightControl.h"
#include "ResourceManager.h"

HudEntityMarkers::HudEntityMarkers(Hud & hud,
                                   Context & context,
                                   const PhysicsWorld & physicsWorld,
                                   const Camera3D & camera):
    m_hud(hud),
    m_context(context),
    m_physicsWorld(physicsWorld),
    m_camera(camera)
{
    auto & resourceManager = hud.world().system<ResourceManager>();

    auto mesh = resourceManager.mesh(R::HudEntityMarkerUpperLeft);
    auto program = resourceManager.program(R::HudElement);

    auto instanceLayout = DataLayout({{"ElementPosition", Type_Vec2}, {"Flip", Type_Vec2}});

    m_instances = LayoutedBlob(instanceLayout);
    m_instanceBuffer = context.createBuffer(instanceLayout);

    m_draw = context.createDraw(program);
    m_draw.setIndices(mesh.indices());
    m_draw.addVertices(mesh.vertices());
    m_draw.addInstanceBuffer(m_instanceBuffer, mesh.indices().count());
    m_draw.sampler("Texture").setTexture(mesh.textures()[0]);
    m_draw.state().setDepthState(DepthState::disabledR());
    m_draw.state().setCullState(CullState::disabled());
    m_draw.state().setBlendState({gl::GL_FUNC_ADD, gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA});
    m_viewportSizeUniform = m_draw.uniform("ViewportSize");
}

void HudEntityMarkers::update()
{
    for (auto & pair : m_markersByEntity)
    {
        pair.second->setVisible(false);
    }
    m_numVisibleMarkers = 0;

    const auto nearPlane = m_camera.nearPlane();
    const auto nearPlaneForward =  m_camera.orientation() * glm::vec3(0.0f, 0.0f, -1.0f);
    const auto nearPlaneRight = m_camera.orientation() * glm::vec3(1.0f, 0.0f, 0.0f);
    const auto nearPlaneUp = m_camera.orientation() * glm::vec3(0.0f, 1.0f, 0.0f);

    for (const auto & rigidBody : m_physicsWorld.rigidBodies())
    {
        auto entity = rigidBody->entity();
        if (entity.hasComponent<PlayerFlightControl>()) continue;

        bool inFrontOfCamera;
        const auto buttonPosition = m_camera.projectToNearPlane(rigidBody->transform().position(), inFrontOfCamera);

        if (!inFrontOfCamera) continue;

        const auto radius = rigidBody->bounds().diameter() / 2.0f;

        const auto upperRightWS = rigidBody->transform().position() + nearPlaneRight * radius + nearPlaneUp * radius;
        const auto upperRightNP = m_camera.projectToNearPlane(upperRightWS, inFrontOfCamera);
        Assert(inFrontOfCamera, "");

        std::shared_ptr<HudButton> button;

        auto iter = m_markersByEntity.find(entity);
        if (iter == m_markersByEntity.end())
        {
            button = std::make_shared<HudButton>();
            iter = m_markersByEntity.emplace(entity, button).first;
            m_hud.addButton(button);
        }

        button = iter->second;

        button->setVisible(true);
        button->setPosition(buttonPosition);
        button->setHalfExtent(upperRightNP - buttonPosition);

        m_numVisibleMarkers++;

        button->setClickCallback([this, entity]() mutable {
            m_hud.setPlayerTarget(entity);
        });
    }
}

void HudEntityMarkers::render()
{
    if (m_numVisibleMarkers == 0) return;

    m_instances.resize(m_numVisibleMarkers * 4); // 4 corners per marker

    auto elementPositions = m_instances.iterator<glm::vec2>("ElementPosition");
    auto flips = m_instances.iterator<glm::vec2>("Flip");

    for (const auto & pair : m_markersByEntity)
    {
        const auto & marker = *pair.second;

        if (!marker.visible()) continue;

        const auto x = marker.halfExtent().x;
        const auto y = marker.halfExtent().y;

        elementPositions.put(marker.position() + glm::vec2(-x, y));
        flips.put({1, 1});
        elementPositions.put(marker.position() + glm::vec2(x, y));
        flips.put({-1, 1});
        elementPositions.put(marker.position() + glm::vec2(x, -y));
        flips.put({-1, -1});
        elementPositions.put(marker.position() + glm::vec2(-x, -y));
        flips.put({1, -1});
    }

    m_instanceBuffer.scheduleUpload(m_instances);

    m_viewportSizeUniform.set(glm::vec2{m_context.backbuffer().width(), m_context.backbuffer().height()});

    m_draw.schedule();
}