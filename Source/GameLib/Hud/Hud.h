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
class HudElement;
class PlayerSystem;

class Hud:
    public std::enable_shared_from_this<Hud>,
    public System<Hud>,
    public InputLayer
{
public:
    Hud(World & world);

    const std::vector<std::shared_ptr<HudLayer>> & layers() const;

    void setPlayerTarget(Entity & entity);

    void addElement(const std::shared_ptr<HudElement> & element);
    void removeElement(const std::shared_ptr<HudElement> & element);

    void onCreated() override { m_input.addLayer(shared_from_this()); }
    void onRemoved() override { m_input.removeLayer(shared_from_this()); }

protected:
    void onUpdate(float seconds) override;

    void onMouseButtonClicked(MouseButtonEvent & event) override;
    void onMouseButtonDown(MouseButtonEvent & event) override;

private:
    template<typename T>
    void processMouseEvent(T & event, const std::function<void(HudElement & element)> & fn) const;

private:
    InputBase &                                 m_input;
    PlayerSystem &                              m_playerSystem;
    std::vector<std::shared_ptr<HudLayer>>      m_layers;
    std::vector<std::shared_ptr<HudElement>>    m_elements;
};