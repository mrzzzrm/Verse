#pragma once

#include <vector>

#include <Deliberation/ECS/Activity.h>
#include <Deliberation/ECS/System.h>

#include <Deliberation/Platform/InputManager.h>
#include <Deliberation/Platform/InputLayer.h>

#include "GameLib.h"

class VoxelShredderSandbox:
    public Activity<VoxelShredderSandbox>
{
public:
    VoxelShredderSandbox();

protected:
    void onKeyPressed(KeyEvent & event) override;

private:
    Entity m_originalEntity;
    std::vector<Entity> m_segmentEntities;
};