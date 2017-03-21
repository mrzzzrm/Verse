#pragma once

#include <functional>

#include <glm/glm.hpp>

#include "GameLib.h"

class HudButton
{
public:
    using ClickCallback = std::function<void()>;

public:
    virtual ~HudButton() = default;

    bool visible() const { return m_visible; }
    const glm::vec2 & position() const { return m_position; }
    const glm::vec2 & halfExtent() const { return m_halfExtent; }
    const ClickCallback & clickCallback() const { return m_clickCallback; }

    void setVisible(bool visible) { m_visible = visible; }
    void setPosition(const glm::vec2 & position) { m_position = position; }
    void setHalfExtent(const glm::vec2 & halfExtent) { m_halfExtent = halfExtent; }
    void setClickCallback(const ClickCallback & clickCallback) { m_clickCallback = clickCallback; }

    void onClick() { if (m_clickCallback) clickCallback(); }

private:
    bool            m_visible = true;
    glm::vec2       m_position;
    glm::vec2       m_halfExtent;
    ClickCallback   m_clickCallback;
};