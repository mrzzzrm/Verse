#include "ColorPalette.h"

#include <Deliberation/Draw/DrawContext.h>

ColorPalette::ColorPalette(
    DrawContext & drawContext, const std::vector<glm::u8vec4> & colors)
    : m_colors(colors)
{
    m_brightnessScales.resize(m_colors.size(), 1.0f);

    m_colorBuffer = drawContext.createBuffer({"Color", Type_U8Vec4});
    m_brightnessScaleBuffer = drawContext.createBuffer({"BrightnessScale", Type_Float});
}

void ColorPalette::setColor(u32 index, const glm::u8vec4 & color)
{
    Assert(index < m_colors.size());
    m_colors[index] = color;
    m_colorsDirty = true;
    m_indicesByColor.clear();
}

void ColorPalette::setBrightnessScale(u32 index, float scale)
{
    Assert(index < m_brightnessScales.size());
    m_brightnessScales[index] = scale;
    m_brightnessScalesDirty = true;
}

const std::unordered_multimap<glm::u8vec3, u32> & ColorPalette::getOrCreateIndicesByColor() const
{
    if (m_indicesByColor.empty())
    {
        for (size_t c = 0; c < m_colors.size(); c++)
        {
            m_indicesByColor.emplace(glm::u8vec3(m_colors[c]), c);
        }
    }

    return m_indicesByColor;
}

void ColorPalette::sync()
{
    if (m_colorsDirty)
    {
        m_colorBuffer.upload(m_colors);
        m_colorsDirty = false;
    }
    if (m_brightnessScalesDirty)
    {
        m_brightnessScaleBuffer.upload(m_brightnessScales);
        m_brightnessScalesDirty = false;
    }
}
