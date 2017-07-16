#pragma once

#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>

#include "GameLib.h"
#include "VoxelClusterMarchingCubesTriangulation.h"
#include "VoxelObject.h"

namespace deliberation
{
class Program;
class Texture;
} // namespace deliberation

class VoxelRenderer : public SingleNodeRenderer
{
public:
    VoxelRenderer(RenderManager & renderManager, const Texture & envMap);

    void addVoxelObject(std::shared_ptr<VoxelObject> voxelObject);
    void removeVoxelObject(std::shared_ptr<VoxelObject> voxelObject);

    void render() override;

private:
    std::vector<std::shared_ptr<VoxelObject>> m_objects;
    u64                                       m_uidIncrementor = 0;
};
