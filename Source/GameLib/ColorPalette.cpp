#include "ColorPalette.h"

#include <Deliberation/Draw/DrawContext.h>

ColorPalette::ColorPalette(DrawContext & drawContext, const std::vector<glm::vec3> & colors):
    m_colors(colors)
{
    auto bufferLayout = DataLayout("Color", Type_Vec3);
    m_buffer = drawContext.createBuffer(bufferLayout);
}

void ColorPalette::setColor(u32 index, const glm::vec3 & color)
{
    Assert(index < m_colors.size(), "");
    m_colors[index] = color;
    m_colorsDirty = true;
}

void ColorPalette::sync()
{
    if (!m_colorsDirty) return;
    m_buffer.upload(m_colors);
    m_colorsDirty = false;
}

