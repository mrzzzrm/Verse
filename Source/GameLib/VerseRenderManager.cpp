#include "VerseRenderManager.h"

VerseRenderManager::VerseRenderManager(World & world, DrawContext & drawContext):
    RenderManager(world, drawContext)
{
    m_ambientLightRenderer = addRenderer<AmbientLightRenderer>();
    m_ssaoRenderer = addRenderer<SsaoRenderer>();
    m_hdrRenderer = addRenderer<HdrRenderer>();
}
