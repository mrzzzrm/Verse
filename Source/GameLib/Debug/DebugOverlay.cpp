#include "DebugOverlay.h"

#include <iostream>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Framebuffer.h>

#include "HailstormManager.h"

DebugOverlay::DebugOverlay(World & world, Context & context):
    Base(world),
    m_font(context, GameDataPath("Data/Font/Xolonium.ttf")),
    m_renderer(context),
    m_labels(m_font)
{
    m_labels.setPosition({-1.0f, 1.0f});
    m_labels.setCenter({-1.0f, 1.0f});
}

void DebugOverlay::setFps(float fps)
{
    m_labels.setText(0, std::to_string((int)fps));
}

void DebugOverlay::onRender()
{
    const auto & profiler = world().profiler();
    const auto numScopes = std::min<size_t>(profiler.scopes().size(), 5u);

    for (size_t s = 0; s < numScopes; s++)
    {
        m_labels.setText(s + 1, profiler.scopes()[s].toString());
    }

    const auto & framebuffer = m_font.context().backbuffer();

    Viewport viewport(0, 0, framebuffer.width(), framebuffer.height());

    m_labels.render(m_renderer, viewport);
}