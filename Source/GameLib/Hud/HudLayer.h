#pragma once

#include "GameLib.h"

class Hud;

class HudLayer
{
public:
    HudLayer(Hud & hud) : m_hud(hud) {}

    virtual ~HudLayer() = default;

    virtual void update(const UpdateFrame & updateFrame) = 0;
    virtual void render() = 0;

protected:
    Hud & m_hud;
};