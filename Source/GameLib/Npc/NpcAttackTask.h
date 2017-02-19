#pragma once

#include <glm/glm.hpp>

#include <Deliberation/ECS/Entity.h>

#include "GameLib.h"
#include "NpcTask.h"

class NpcAttackTask:
    public NpcTask
{
public:
    NpcAttackTask();

    void setTarget(Entity target);

    void update(NpcController & controller, float seconds) override;

private:
    enum class Status
    {
        None,
        Joust,
        Evade
    };

private:
    void startEvasion(NpcController & controller);
    void startJoust();

private:
    Entity      m_target;
    Status      m_status = Status::None;
    glm::vec3   m_evasionPoint;
};