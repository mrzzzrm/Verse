#pragma once

#include <memory>

#include <Deliberation/ECS/Component.h>

#include "Equipment.h"
#include "GameLib.h"
#include "NpcTask.h"
#include "NpcSteering.h"
#include "NpcFlightControl.h"

class NpcFlightControl;

class NpcController final:
    public Component<NpcController>
{
public:
    NpcSteering & steering();
    const NpcSteering & steering() const;
    const std::shared_ptr<NpcTask> & task() const;

    void setTask(std::shared_ptr<NpcTask> task);

    void update(RigidBody & body,
                NpcFlightControl & flightControl,
                const FlightControlConfig & config,
                float seconds);

private:
    NpcSteering                         m_steering;
    std::shared_ptr<NpcTask>            m_task;
};