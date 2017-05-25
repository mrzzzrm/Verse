#pragma once

#include <vector>

#include <glm/glm.hpp>

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
    ColorPalette(DrawContext & drawContext, const std::vector<glm::vec3> & colors);

    const std::vector<glm::vec3> & colors() const { return m_colors; }
    const Buffer & colorBuffer() const { return m_buffer; }

    void setColor(u32 index, const glm::vec3 & color);

    void sync();

private:
    std::vector<glm::vec3> m_colors;
    bool m_colorsDirty = true;
    Buffer m_buffer;
};