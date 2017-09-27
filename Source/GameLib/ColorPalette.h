#pragma once

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <Deliberation/Core/IntTypes.h>

#include <Deliberation/Draw/Buffer.h>

#include "GameLib.h"

namespace deliberation
{
class DrawContext;
}

class ColorPalette final
{
public:
    ColorPalette(const std::vector<glm::u8vec4> & colors);

    const std::vector<glm::u8vec4> & colors() const { return m_colors; }
    const Buffer &                   colorBuffer() const { return m_colorBuffer; }

    const std::vector<float> & brightnessScales() const { return m_brightnessScales; }
    const Buffer &             brightnessScaleBuffer() const { return m_brightnessScaleBuffer; }

    const std::unordered_multimap<glm::u8vec3, u32> & getOrCreateIndicesByColor() const;

    void setColor(u32 index, const glm::u8vec4 & color);
    void setBrightnessScale(u32 index, float scale);

    void sync();

private:
    std::vector<glm::u8vec4>    m_colors;
    bool                        m_colorsDirty = true;
    Buffer                      m_colorBuffer;

    std::vector<float>          m_brightnessScales;
    bool                        m_brightnessScalesDirty = true;
    Buffer                      m_brightnessScaleBuffer;

    mutable std::unordered_multimap<glm::u8vec3, u32>
                                m_indicesByColor;
};