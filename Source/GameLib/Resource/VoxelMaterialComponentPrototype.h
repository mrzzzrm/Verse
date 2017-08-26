#pragma once

#include <Deliberation/ECS/ComponentPrototype.h>

#include "VoxelMaterialComponent.h"

class VoxelMaterialComponentPrototype:
    public ComponentPrototype<VoxelMaterialComponent>
{
public:
    void updateComponent(const Entity & entity, VoxelMaterialComponent & voxelMaterialComponent) override;
};
