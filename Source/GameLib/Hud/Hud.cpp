#include "Hud.h"

#include <algorithm>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/Systems/PhysicsWorldSystem.h>
#include <Deliberation/ECS/World.h>

#include "HudButton.h"
#include "PlayerSystem.h"

Hud::Hud(World & world, const Camera3D & camera):
    Base(world),
    InputLayer(1),
    m_input(world.system<ApplicationSystem>().input()),
    m_entityMarkers(*this,
                    world.system<ApplicationSystem>().context(),
                    world.system<PhysicsWorldSystem>().physicsWorld(),
                    camera),
    m_playerSystem(world.system<PlayerSystem>())
{

}

void Hud::setPlayerTarget(Entity & entity)
{
    m_playerSystem.setPlayerTarget(entity);
}

void Hud::addButton(const std::shared_ptr<HudButton> & button)
{
    m_buttons.emplace_back(button);
}

void Hud::removeButton(const std::shared_ptr<HudButton> & button)
{
    const auto iter = std::find(m_buttons.begin(), m_buttons.end(), button);
    Assert(iter != m_buttons.end(), "No such button");

    m_buttons.erase(iter);
}

void Hud::onUpdate(float seconds)
{
    m_entityMarkers.update();
}

void Hud::onRender()
{
    m_entityMarkers.render();
}

void Hud::onMouseButtonClicked(MouseButtonEvent & event)
{
    for (auto & button : m_buttons)
    {
        if (!button->visible()) continue;

        const auto & halfExtent = button->halfExtent();
        const auto absMouseDelta = glm::abs(event.mousePosition() - button->position());

        std::cout << absMouseDelta << " " << halfExtent << std::endl;

        if (absMouseDelta.x <= halfExtent.x && absMouseDelta.y <= halfExtent.y)
        {
            event.consume();
            button->onClick();
            break;
        }
    }
}