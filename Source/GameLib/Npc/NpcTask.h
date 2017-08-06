#pragma once

#include "GameLib.h"

class Equipment;
class NpcController;

namespace deliberation
{
class UpdateFrame;
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
        const UpdateFrame & updateFrame) = 0;
};