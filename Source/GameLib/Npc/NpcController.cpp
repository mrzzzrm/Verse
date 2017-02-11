#include "NpcController.h"

#include <Deliberation/Core/Assert.h>

NpcController::NpcController(FlightControlConfig flightControlConfig):
    m_flightControlConfig(flightControlConfig)
{}

const std::shared_ptr<RigidBody> & NpcController::body() const
{
    return m_flightControl->body();
}

NpcSteering & NpcController::steering()
{
    return m_steering;
}

const NpcSteering & NpcController::steering() const
{
    return m_steering;
}

const std::shared_ptr<NpcFlightControl> & NpcController::flightControl()
{
    return m_flightControl;
}

std::shared_ptr<const NpcFlightControl> NpcController::flightControl() const
{
    return m_flightControl;
}

void NpcController::setBody(std::shared_ptr<RigidBody> body)
{
    m_flightControl = std::make_shared<NpcFlightControl>(body, m_flightControlConfig);
}

void NpcController::setTask(std::shared_ptr<NpcTask> task)
{
    m_task = task;
}

void NpcController::update(float seconds)
{
    Assert(!m_task || m_flightControl, "Task but not flight control");
    if (m_task) m_task->update(*this, seconds);

    if (m_flightControl)
    {
        m_steering.update(*m_flightControl, seconds);
        m_flightControl->update(seconds);
    }
}