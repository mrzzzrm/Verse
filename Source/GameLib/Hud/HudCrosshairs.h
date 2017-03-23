#pragma once

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Uniform.h>

#include "GameLib.h"
#include "HudElement.h"
#include "HudLayer.h"

class PlayerSystem;

class HudCrosshairs:
    public HudLayer,
    public HudElement
{
public:
    HudCrosshairs(Hud & hud);

    void update(float seconds) override;
    void render() override;

    void onMouseButtonDown(MouseButtonEvent & event) override;

private:
    PlayerSystem &  m_playerSystem;
    Context &       m_context;
    Draw            m_draw;
    Uniform         m_viewportSizeUniform;
    Uniform         m_crosshairPositionUniform;

};