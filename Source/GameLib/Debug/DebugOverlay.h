#pragma once

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Font/Label.h>
#include <Deliberation/Font/LabelRenderer.h>
#include <Deliberation/Font/MultilineLabel.h>
#include <Deliberation/Font/Font.h>

#include "GameLib.h"

class DebugOverlay:
    public System<DebugOverlay>
{
public:
    DebugOverlay(World & world, Context & context);

    void setFps(float fps);

protected:
    void onRender() override;

private:
    Font            m_font;
    LabelRenderer   m_renderer;

    MultilineLabel  m_labels;
};