#include "HudEntityMarkers.h"

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Faction/Allegiance.h>

#include "FactionManager.h"
#include "Hud.h"
#include "HudButton.h"
#include "HudProxy.h"
#include "PlayerFlightControl.h"
#include "PlayerSystem.h"
#include "ResourceManager.h"

HudEntityMarkers::HudEntityMarkers(Hud & hud,
                                   DrawContext & context,
                                   const PhysicsWorld & physicsWorld,
                                   const Camera3D & camera):
    HudLayer(hud),
    m_physicsWorld(physicsWorld),
    m_camera(camera),
    m_renderer(context, hud.world().system<ResourceManager>())
{
}

void HudEntityMarkers::update(float seconds)
{
    for (auto & pair : m_markersByEntity)
    {
        pair.second->setVisible(false);
    }
    m_numVisibleMarkers = 0;

    auto & playerSystem = m_hud.world().system<PlayerSystem>();
    auto & factionManager = m_hud.world().system<FactionManager>();

    const auto nearPlane = m_camera.nearPlane();
    const auto nearPlaneForward =  m_camera.orientation() * glm::vec3(0.0f, 0.0f, -1.0f);
    const auto nearPlaneRight = m_camera.orientation() * glm::vec3(1.0f, 0.0f, 0.0f);
    const auto nearPlaneUp = m_camera.orientation() * glm::vec3(0.0f, 1.0f, 0.0f);
    const auto * playerAllegiance = (Allegiance*)nullptr;

    if (playerSystem.player().isValid())
    {
        if (playerSystem.player().hasComponent<Allegiance>())
        {
            playerAllegiance = &playerSystem.player().component<Allegiance>();
        }
    }

    for (const auto & rigidBody : m_physicsWorld.rigidBodies())
    {
        auto entity = rigidBody->entity();
        if (entity.hasComponent<PlayerFlightControl>()) continue;
        if (!entity.hasComponent<HudProxy>()) continue;

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
            m_hud.addElement(button);
        }

        button = iter->second;

        button->setVisible(true);
        button->setPosition(buttonPosition);
        button->setHalfExtent(upperRightNP - buttonPosition);

        /**
         * Set color based on allegiance
         */
        auto color = glm::vec3(0.5f, 0.5f, 0.5f);
        if (playerAllegiance != nullptr && entity.hasComponent<Allegiance>())
        {
            auto entityAllegiance = entity.component<Allegiance>();

            auto factionRelation = factionManager.factionRelation(playerAllegiance->faction(), entityAllegiance.faction());

            if (factionRelation == FactionRelation::Hostile)
            {
                color = glm::vec3(1.0f, 0.2f, 0.0f);
            }
        }
        button->setColor(color);

        m_numVisibleMarkers++;

        button->setClickCallback([this, entity]() mutable {
            m_hud.setPlayerTarget(entity);
        });
    }
}

void HudEntityMarkers::render()
{
    if (m_numVisibleMarkers == 0) return;

    m_visibleMarkers.clear();

    for (const auto & pair : m_markersByEntity)
    {
        const auto & marker = pair.second;

        if (!marker->visible()) continue;

        m_visibleMarkers.emplace_back(marker);
    }

    m_renderer.render(m_visibleMarkers);
}