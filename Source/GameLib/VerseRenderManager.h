#pragma once

#include <Deliberation/Scene/HdrRenderer.h>
#include <Deliberation/Scene/Lighting/AmbientLightRenderer.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/SsaoRenderer.h>

#include "GameLib.h"

class VerseRenderManager : public RenderManager
{
public:
    VerseRenderManager();

private:
    std::shared_ptr<AmbientLightRenderer> m_ambientLightRenderer;
    std::shared_ptr<SsaoRenderer>         m_ssaoRenderer;
    std::shared_ptr<HdrRenderer>          m_hdrRenderer;
};