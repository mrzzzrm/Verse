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

        const auto relativePosition = rigidBody->transform().position() - m_camera.position();

        if (glm::dot(relativePosition, nearPlaneForward) < 0) continue;

        const auto radius = rigidBody->bounds().diameter() / 2.0f;

        const auto localRelativePosition = glm::transpose(m_camera.pose().basis()) * relativePosition;

        const auto factor = m_camera.zNear() / -localRelativePosition.z;

        const auto nearPlanePositionWS = (localRelativePosition * factor);
        const auto nearPlanePosition2dWS = glm::vec2(nearPlanePositionWS.x, nearPlanePositionWS.y);
        const auto nearPlanePosition = 2.0f * nearPlanePosition2dWS / nearPlane.size();

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
        button->setPosition(nearPlanePosition);
        button->setHalfExtent(glm::vec2((radius / glm::root_two<float>()) / 2.0f));

        button->setClickCallback([&]() {
            m_hud.setPlayerTarget(entity);
        });
    }

}

void HudEntityMarkers::render()
{

}