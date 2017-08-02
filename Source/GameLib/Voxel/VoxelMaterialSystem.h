#pragma once

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class VoxelObject;
class VoxelMaterialComponent;

class VoxelMaterialSystem:
    public System<VoxelMaterialSystem>
{
public:
    VoxelMaterialSystem(World & world);

protected:
    void onEntityGameUpdate(Entity & entity, float seconds) override;
    void onEntityAdded(Entity & entity) override;

private:
    void updateBrightnessScales(VoxelObject & voxelObject,
                                               VoxelMaterialComponent & voxelMaterialComponent);
};