#include "Hud.h"

#include <algorithm>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/Systems/PhysicsWorldSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderSystem.h>

#include "HudButton.h"
#include "HudCrosshairs.h"
#include "HudRenderer.h"
#include "PlayerSystem.h"

Hud::Hud(World & world)
    : Base(world)
    , InputLayer(1)
    , m_input(world.systemRef<ApplicationSystem>().input())
    , m_playerSystem(world.systemRef<PlayerSystem>())
{
    auto & context = world.systemRef<ApplicationSystem>().drawContext();
    auto & physicsWorld = world.systemRef<PhysicsWorldSystem>().physicsWorld();

    auto crosshairs = std::make_shared<HudCrosshairs>(*this);

    auto & renderManager = world.systemRef<RenderSystem>().renderManager();

    renderManager.addRenderer<HudRenderer>(*this);

    m_layers.emplace_back(crosshairs);
    m_layers.emplace_back(std::make_shared<HudEntityMarkers>(
        *this, context, physicsWorld, renderManager.mainCamera()));

    addElement(crosshairs);
}

void Hud::setPlayerTarget(Entity & entity)
{
    m_playerSystem.setPlayerTarget(entity);
}

void Hud::addElement(const std::shared_ptr<HudElement> & element)
{
    m_elements.emplace_back(element);
}

void Hud::removeElement(const std::shared_ptr<HudElement> & element)
{
    const auto iter = std::find(m_elements.begin(), m_elements.end(), element);
    Assert(iter != m_elements.end(), "No such button");

    m_elements.erase(iter);
}

void Hud::onGameUpdate(float seconds)
{
    for (auto & layer : m_layers)
    {
        layer->update(seconds);
    }
}

void Hud::onMouseButtonClicked(MouseButtonEvent & event)
{
    processMouseEvent(event, [&](HudElement & element) {
        element.onMouseButtonClicked(event);
    });
}

void Hud::onMouseButtonPressed(MouseButtonEvent & event)
{
    processMouseEvent(event, [&](HudElement & element) {
        element.onMouseButtonPressed(event);
    });
}

template<typename T>
void Hud::processMouseEvent(
    T & event, const std::function<void(HudElement & element)> & fn) const
{
    for (auto & element : m_elements)
    {
        if (!element->visible()) continue;

        const auto & halfExtent = element->halfExtent();
        const auto   absMouseDelta =
            glm::abs(event.mousePosition() - element->position());

        if (absMouseDelta.x <= halfExtent.x && absMouseDelta.y <= halfExtent.y)
        {
            fn(*element);
            if (event.isConsumed()) break;
        }
    }
}