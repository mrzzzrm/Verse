#include "VerseRenderManager.h"

VerseRenderManager::VerseRenderManager(DrawContext & drawContext)
    : RenderManager(drawContext)
{
    m_ambientLightRenderer = addRenderer<AmbientLightRenderer>();
    m_ssaoRenderer = addRenderer<SsaoRenderer>();
    m_hdrRenderer = addRenderer<HdrRenderer>();
}
