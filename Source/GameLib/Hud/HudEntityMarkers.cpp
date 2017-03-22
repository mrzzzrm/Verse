#include "HudEntityMarkers.h"

#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>

#include <Deliberation/Scene/Camera3D.h>

#include "Hud.h"
#include "HudButton.h"
#include "PlayerFlightControl.h"

HudEntityMarkers::HudEntityMarkers(Hud & hud,
                                   Context & context,
                                   const PhysicsWorld & physicsWorld,
                                   const Camera3D & camera):
    m_hud(hud),
    m_physicsWorld(physicsWorld),
    m_camera(camera)
{

}

void HudEntityMarkers::update()
{
    for (auto & pair : m_markersByEntity)
    {
        pair.second->setVisible(false);
    }

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

        button->setClickCallback([&]() {
            m_hud.setPlayerTarget(entity);
        });
    }

}

void HudEntityMarkers::render()
{

}