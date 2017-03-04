#pragma once

#include <memory>

#include <Deliberation/ECS/Component.h>

#include "Equipment.h"
#include "GameLib.h"
#include "NpcTask.h"
#include "NpcSteering.h"
#include "NpcFlightControl.h"

class NpcController final:
    public Component<NpcController>
{
public:
    NpcController(FlightControlConfig config);

    const std::shared_ptr<RigidBody> & body() const;
    NpcSteering & steering();
    const NpcSteering & steering() const;
    const std::shared_ptr<NpcFlightControl> & flightControl();
    std::shared_ptr<const NpcFlightControl> flightControl() const;

    void setBody(std::shared_ptr<RigidBody> body);
    void setTask(std::shared_ptr<NpcTask> task);

    void update(float seconds, Equipment & equipment);

private:
    FlightControlConfig                 m_flightControlConfig;
    std::shared_ptr<NpcFlightControl>   m_flightControl;
    NpcSteering                         m_steering;
    std::shared_ptr<NpcTask>            m_task;
    std::shared_ptr<Equipment>          m_equipment;
};