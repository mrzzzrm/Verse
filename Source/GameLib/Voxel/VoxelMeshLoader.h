#pragma once

#include <memory>
#include <string>

#include "GameLib.h"

namespace deliberation
{
class MeshData;
}

class ResourceManager;
class VoxelWorld;

class VoxelMeshLoader
{
public:
    VoxelMeshLoader(const std::shared_ptr<ResourceManager> & resourceManager,
                    const std::shared_ptr<VoxelWorld> & voxelWorld);

    std::shared_ptr<MeshData> load(const std::string & path);

private:
    std::shared_ptr<ResourceManager> m_resourceManager;
    std::shared_ptr<VoxelWorld> m_voxelWorld;
};