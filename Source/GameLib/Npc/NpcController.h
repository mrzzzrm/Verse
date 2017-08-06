#pragma once

#include <memory>

#include <Deliberation/ECS/Component.h>

#include "Equipment.h"
#include "GameLib.h"
#include "NpcFlightControl.h"
#include "NpcSteering.h"
#include "NpcTask.h"

class NpcFlightControl;

class NpcController final : public Component<NpcController>
{
    DELIBERATION_COMPONENT_NAME("NpcController")
public:
    NpcSteering &                    steering();
    const NpcSteering &              steering() const;
    const std::shared_ptr<NpcTask> & task() const;

    void setTask(std::shared_ptr<NpcTask> task);

    void update(
        RigidBody &                 body,
        NpcFlightControl &          flightControl,
        const FlightControlConfig & config,
        const UpdateFrame & updateFrame);

private:
    NpcSteering              m_steering;
    std::shared_ptr<NpcTask> m_task;
};