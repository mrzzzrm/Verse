#include "VerseResourceManager.h"

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
            return VoxelMeshLoader(shared_from_this(), world().system<VoxelWorld>()).load(path);
        } else {
            Fail("Only voxel meshes supported right now, can't load '" + path + "'");
            return std::shared_ptr<MeshData>();
        }
    });
    setLoader<VoxReader::VoxelModels>([&] (const std::string & path) -> VoxReader::VoxelModels{
        auto models = VoxReader().read(path);
        std::cout << "Loaded " << models.size() << " models";
        return models;
    });

    registerBuildIns();
}