#pragma once

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Uniform.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>

#include "GameLib.h"
#include "HudElement.h"
#include "HudLayer.h"

class PlayerSystem;

class HudCrosshairs : public HudLayer, public HudElement
{
public:
    HudCrosshairs(Hud & hud);

    void update(float seconds) override;
    void render() override;

    void onMouseButtonPressed(MouseButtonEvent & event) override;

private:
    PlayerSystem &  m_playerSystem;
    RenderManager & m_renderManager;
    DrawContext &   m_drawContext;
    Draw            m_draw;
    Uniform         m_viewportSizeUniform;
    Uniform         m_crosshairPositionUniform;
    glm::vec3       m_trajectory;
};