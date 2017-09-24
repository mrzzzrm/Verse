#pragma once

#include <memory>
#include <string>

#include "GameLib.h"

namespace deliberation
{
class MeshData;
class ResourceManager;
}

class VoxelWorld;

class VoxelMeshLoader
{
public:
    std::shared_ptr<MeshData> load(const std::string & path);
};