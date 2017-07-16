#include "HudRenderer.h"

#include <Deliberation/Scene/Pipeline/RenderPhase.h>

#include "Hud.h"

HudRenderer::HudRenderer(RenderManager & renderManager, Hud & hud)
    : SingleNodeRenderer(renderManager, RenderPhase::PostAlpha), m_hud(hud)
{
}

void HudRenderer::render()
{
    for (auto & layer : m_hud.layers())
    {
        layer->render();
    }
}