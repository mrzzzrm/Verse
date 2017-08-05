#pragma once

#include <Deliberation/ECS/System.h>
#include <Deliberation/Resource/PrototypesReloadedEvent.h>

#include "GameLib.h"

class VoxelObject;
class VoxelMaterialComponent;

class VoxelMaterialSystem:
    public System<VoxelMaterialSystem>
{
public:
    VoxelMaterialSystem(World & world);

    void onCreated() override;
    void onEvent(const PrototypesReloadedEvent & event);

protected:
    void onEntityGameUpdate(Entity & entity, float seconds) override;
    void onEntityAdded(Entity & entity) override;

private:
    void updateBrightnessScales(VoxelObject & voxelObject,
                                               VoxelMaterialComponent & voxelMaterialComponent);
};