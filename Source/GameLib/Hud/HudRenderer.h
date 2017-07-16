#pragma once

#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>

#include "GameLib.h"
#include "Hud.h"

class HudRenderer : public SingleNodeRenderer
{
public:
    HudRenderer(RenderManager & renderManager, Hud & hud);

    void render() override;

private:
    Hud & m_hud;
};