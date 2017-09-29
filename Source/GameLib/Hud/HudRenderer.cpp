#include "HudRenderer.h"

#include <Deliberation/Scene/Pipeline/RenderPhase.h>

#include "Hud.h"

HudRenderer::HudRenderer(Hud & hud)
    : SingleNodeRenderer(RenderPhase::PreOverlay, "HUD"), m_hud(hud)
{
}

void HudRenderer::render()
{
    for (auto & layer : m_hud.layers())
    {
        layer->render();
    }
}