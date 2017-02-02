#pragma once

#include <memory>

#include "GameLib.h"
#include "FlightControl.h"
#include "NpcTask.h"

class NpcController final
{
public:
    void setTask(std::shared_ptr<NpcTask> task);

    void update(float seconds);

private:
    std::shared_ptr<NpcTask> m_task;
};