#pragma once

class Equipment;
class NpcController;

class NpcTask
{
public:
    virtual ~NpcTask() = default;
    virtual void update(NpcController & controller, Equipment & equipment, float seconds) = 0;
};