#pragma once

#include <Deliberation/Core/InputBase.h>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Platform/InputLayer.h>

#include "GameLib.h"
#include "HudEntityMarkers.h"

namespace deliberation
{
class Camera3D;
}

class HudButton;
class PlayerSystem;

class Hud:
    public std::enable_shared_from_this<Hud>,
    public System<Hud>,
    public InputLayer
{
public:
    Hud(World & world, const Camera3D & camera);

    void setPlayerTarget(Entity & entity);

    void addButton(const std::shared_ptr<HudButton> & button);
    void removeButton(const std::shared_ptr<HudButton> & button);

    void onCreated() override { m_input.addLayer(shared_from_this()); }
    void onRemoved() override { m_input.removeLayer(shared_from_this()); }

protected:
    void onUpdate(float seconds) override;
    void onRender() override;

    void onMouseButtonClicked(MouseButtonEvent & event) override;

private:
    InputBase &                             m_input;
    PlayerSystem &                          m_playerSystem;
    HudEntityMarkers                        m_entityMarkers;
    std::vector<std::shared_ptr<HudButton>> m_buttons;
};