#pragma once

#include <Deliberation/Core/Chrono.h>
#include <Deliberation/Core/UpdateFrame.h>

#include "AbstractBehaviour.h"

class LaunchDefenseBehaviour : public AbstractBehaviour
{
public:
    void onBehaviourUpdate(const UpdateFrame & updateFrame) override;

private:
    u32             m_numLaunched = 0;
    TimestampMillis m_lastLaunchedMillis = 0;
};