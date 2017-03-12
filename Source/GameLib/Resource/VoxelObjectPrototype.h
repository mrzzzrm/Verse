#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/World.h>

#include "ComponentPrototype.h"
#include "GameLib.h"
#include "VoxelWorld.h"

class VoxelObjectVoxelData;
class VoxelWorld;

class VoxelObjectPrototype final:
    public ComponentPrototype
{
public:
    VoxelObjectPrototype(const nlohmann::json & json, VoxelWorld & voxelWorld);

    void applyToEntity(Entity & entity) const override;

private:
    std::shared_ptr<VoxelObjectVoxelData>   m_voxelDataPrototype;
};