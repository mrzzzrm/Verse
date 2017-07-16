#pragma once

#include <Deliberation/Core/Chrono.h>

#include "AbstractBehaviour.h"

class LaunchDefenseBehaviour : public AbstractBehaviour
{
public:
    void onBehaviourUpdate(float seconds) override;

private:
    u32             m_numLaunched = 0;
    TimestampMillis m_lastLaunchedMillis = 0;
};