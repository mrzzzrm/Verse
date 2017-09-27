#pragma once

#include <Deliberation/Core/Chrono.h>
#include <Deliberation/Core/UpdateFrame.h>

#include <Deliberation/ECS/AbstractActivity.h>
#include <Deliberation/ECS/Activity.h>

#include "GameLib.h"

class LaunchDefenseActivity : public Activity<LaunchDefenseActivity>
{
public:
    void onCreated() override;

    void onGameUpdate(const UpdateFrame & updateFrame);

protected:
    void onAttachedToEntity() override;

private:
    u32             m_numLaunched = 0;
    TimestampMillis m_lastLaunchedMillis = 0;
};