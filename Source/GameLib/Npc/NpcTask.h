#pragma once

#include "GameLib.h"

class Equipment;
class NpcController;

namespace deliberation
{
class RigidBody;
}

class NpcTask
{
public:
    virtual ~NpcTask() = default;
    virtual void update(
        NpcController & controller,
        RigidBody &     body,
        Equipment &     equipment,
        float           seconds) = 0;
};