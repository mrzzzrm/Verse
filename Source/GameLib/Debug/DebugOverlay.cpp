#include "DebugOverlay.h"

#include <iostream>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Framebuffer.h>

DebugOverlay::DebugOverlay(World & world, Context & context):
    Base(world),
    m_font(context, GameDataPath("Data/Font/Xolonium.ttf")),
    m_renderer(context),
    m_fpsLabel(m_font)
{
    m_fpsLabel.setText("FPS: --");
    m_fpsLabel.setPosition({-1, 1});
    m_fpsLabel.setCenter({-1, 1});
}

void DebugOverlay::setFps(float fps)
{
    m_fpsLabel.setText("FPS: " + std::to_string((int)fps));
}

void DebugOverlay::onRender()
{
    const auto & framebuffer = m_font.context().backbuffer();

    Viewport viewport(0, 0, framebuffer.width(), framebuffer.height());
    m_renderer.render(m_fpsLabel, viewport);
}