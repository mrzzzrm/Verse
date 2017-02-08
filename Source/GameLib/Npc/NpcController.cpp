#include "NpcController.h"

void NpcController::setTask(std::shared_ptr<NpcTask> task)
{
    m_task = task;
}

void NpcController::update(float seconds)
{
    if (m_task) m_task->update(seconds);
}