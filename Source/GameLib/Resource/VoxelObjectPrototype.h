#pragma once

#include <memory>

#include <boost/optional.hpp>

#include <glm/glm.hpp>

#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/ComponentPrototype.h>
#include <Deliberation/ECS/World.h>

#include "GameLib.h"
#include "VoxelWorld.h"

class VoxelObjectVoxelData;
class VoxelWorld;

class VoxelObjectPrototype final : public ComponentPrototype<VoxelObject>
{
public:
    VoxelObjectPrototype(const std::shared_ptr<VoxelWorld> & voxelWorld);

    void initComponent(VoxelObject & voxelObject) override;
    void updateComponent(VoxelObject & voxelObject) override {}

private:
    std::shared_ptr<VoxelWorld>           m_voxelWorld;
    float                                 m_scale = 1.0f;
    boost::optional<glm::uvec3>           m_crucialVoxel;
    std::shared_ptr<VoxelObjectVoxelData> m_voxelDataPrototype;
};