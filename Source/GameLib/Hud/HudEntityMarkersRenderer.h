#pragma once

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>

#include "GameLib.h"

namespace deliberation
{
class ResourceManager;
class DrawContext;
}

class HudButton;

class HudEntityMarkersRenderer
{
public:
    HudEntityMarkersRenderer(
        DrawContext & context);

    void render(const std::vector<std::shared_ptr<HudButton>> & markers);

private:
    DrawContext & m_drawContext;
    Draw          m_draw;
    Uniform       m_viewportSizeUniform;
    LayoutedBlob  m_instances;
    Buffer        m_instanceBuffer;
};