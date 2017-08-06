#include "NpcController.h"

#include <Deliberation/Core/Assert.h>

NpcSteering & NpcController::steering() { return m_steering; }

const NpcSteering & NpcController::steering() const { return m_steering; }

const std::shared_ptr<NpcTask> & NpcController::task() const { return m_task; }

void NpcController::setTask(std::shared_ptr<NpcTask> task) { m_task = task; }

void NpcController::update(
    RigidBody &                 body,
    NpcFlightControl &          flightControl,
    const FlightControlConfig & config,
    const UpdateFrame & updateFrame)
{
    m_steering.update(body, flightControl, config, updateFrame);
}