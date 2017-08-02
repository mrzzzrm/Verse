#pragma once

#include <Deliberation/ECS/ComponentPrototype.h>

#include "VoxelMaterialComponent.h"

class VoxelMaterialComponentPrototype:
    public ComponentPrototype<VoxelMaterialComponent>
{
public:
    void updateComponent(VoxelMaterialComponent & voxelMaterialComponent) override;
};
