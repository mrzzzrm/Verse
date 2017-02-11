#pragma once

class NpcController;

class NpcTask
{
public:
    virtual ~NpcTask() = default;
    virtual void update(NpcController & controller, float seconds) = 0;
};