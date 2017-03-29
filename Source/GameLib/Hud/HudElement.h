#pragma once

#include <Deliberation/Platform/InputEventReceiver.h>

#include "GameLib.h"

class HudElement:
    public InputEventReceiver
{
public:
    virtual ~HudElement() = default;

    bool visible() const { return m_visible; }
    const glm::vec3 & color() const { return m_color; }
    const glm::vec2 & position() const { return m_position; }
    const glm::vec2 & halfExtent() const { return m_halfExtent; }

    void setVisible(bool visible) { m_visible = visible; }
    void setColor(const glm::vec3 & color) { m_color = color; }
    void setPosition(const glm::vec2 & position) { m_position = position; }
    void setHalfExtent(const glm::vec2 & halfExtent) { m_halfExtent = halfExtent; }

private:
    bool        m_visible = true;
    glm::vec3   m_color = glm::vec3(1.0f);
    glm::vec2   m_position;
    glm::vec2   m_halfExtent;
};