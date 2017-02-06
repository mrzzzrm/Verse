#include "NpcTask.h"

#include <iostream>

NpcTask::NpcTask(std::shared_ptr<NpcFlightControl> flightControl):
    m_flightControl(flightControl)
{
    std::cout << "Setting PlayerFlightControl: " << m_flightControl << std::endl;
}