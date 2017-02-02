#pragma once

#include <memory>

#include "FlightControl.h"

class NpcTask
{
public:
    NpcTask(std::shared_ptr<FlightControl> flightControl);
    virtual ~NpcTask() = default;
    virtual void update(float seconds) = 0;

protected:
    std::shared_ptr<FlightControl> m_flightControl;
};