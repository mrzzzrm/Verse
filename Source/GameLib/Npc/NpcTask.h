#pragma once

#include <memory>

#include "NpcFlightControl.h"

class NpcTask
{
public:
    NpcTask(std::shared_ptr<NpcFlightControl> flightControl);
    virtual ~NpcTask() = default;
    virtual void update(float seconds) = 0;

protected:
    std::shared_ptr<NpcFlightControl> m_flightControl;
};