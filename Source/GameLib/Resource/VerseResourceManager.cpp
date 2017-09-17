#include "VerseResourceManager.h"

#include <Deliberation/Core/Log.h>
#include <Deliberation/Core/StringUtils.h>
#include <Deliberation/ECS/World.h>

#include "VoxelWorld.h"
#include "VoxelMeshLoader.h"
#include "VoxReader.h"

VerseResourceManager::VerseResourceManager(World & world):
    ResourceManager(world)
{
}

void VerseResourceManager::onCreated()
{
    setLoader<std::shared_ptr<MeshData>>([&] (const auto & path) {
        if (StringEndsWith(path, ".vox")) {
            return VoxelMeshLoader(this->shared_from_this(), this->world().system<VoxelWorld>()).load(path);
        } else {
            Fail("Only voxel meshes supported right now, can't load '" + path + "'");
            return std::shared_ptr<MeshData>();
        }
    });
    setLoader<VoxReader::VoxelModels>([&] (const std::string & path) -> VoxReader::VoxelModels{
        auto models = VoxReader().read(path);
        Log->info("Loaded {} models", models.size());
        return models;
    });

    registerBuildIns();
}