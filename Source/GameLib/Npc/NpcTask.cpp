#include "NpcTask.h"

#include <iostream>

NpcTask::NpcTask(std::shared_ptr<FlightControl> flightControl):
    m_flightControl(flightControl)
{
    std::cout << "Setting FlightControl: " << m_flightControl << std::endl;
}