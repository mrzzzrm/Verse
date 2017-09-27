#include "VerseRenderManager.h"

VerseRenderManager::VerseRenderManager()
{
    m_ambientLightRenderer = addRenderer<AmbientLightRenderer>();
    m_ssaoRenderer = addRenderer<SsaoRenderer>();
    m_hdrRenderer = addRenderer<HdrRenderer>();
}
