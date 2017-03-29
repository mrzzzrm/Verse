#pragma once

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>

#include "GameLib.h"

namespace deliberation
{
class Context;
}

class HudButton;
class ResourceManager;

class HudEntityMarkersRenderer
{
public:
    HudEntityMarkersRenderer(Context & context, ResourceManager & resourceManager);

    void render(const std::vector<std::shared_ptr<HudButton>> & markers);

private:
    Context &                       m_context;
    Draw                            m_draw;
    Uniform                         m_viewportSizeUniform;
    LayoutedBlob                    m_instances;
    Buffer                          m_instanceBuffer;
};