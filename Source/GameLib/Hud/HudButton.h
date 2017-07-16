#pragma once

#include <functional>

#include <glm/glm.hpp>

#include "GameLib.h"
#include "HudElement.h"

class HudButton : public HudElement
{
public:
    using ClickCallback = std::function<void()>;

public:
    virtual ~HudButton() = default;

    const ClickCallback & clickCallback() const { return m_clickCallback; }

    void setClickCallback(const ClickCallback & clickCallback)
    {
        m_clickCallback = clickCallback;
    }

    void onMouseButtonClicked(MouseButtonEvent & event) override;

private:
    ClickCallback m_clickCallback;
};