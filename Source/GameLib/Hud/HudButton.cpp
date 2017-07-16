#include "HudButton.h"

void HudButton::onMouseButtonClicked(MouseButtonEvent & event)
{
    if (event.button() == MouseButton::Left && m_clickCallback)
    {
        m_clickCallback();
        event.consume();
    }
}