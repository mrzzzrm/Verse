#include "VoxelObjectPrototype.h"

#include "ColorPalette.h"
#include "VoxReader.h"
#include "VoxelObject.h"
#include "VoxelObjectVoxelData.h"

VoxelObjectPrototype::VoxelObjectPrototype(
    const std::shared_ptr<VoxelWorld> & voxelWorld)
    : m_voxelWorld(voxelWorld)
{
}

void VoxelObjectPrototype::initComponent(const Entity & entity, VoxelObject & voxelObject)
{
    const auto voxelClusterName = m_newJson["VoxelCluster"].get<std::string>();

    VoxReader voxReader;
    {
        auto models = voxReader.read(
            GameDataPath("Data/VoxelClusters/" + voxelClusterName + ".vox"));
        if (!models.empty())
        {
            auto palette = std::make_shared<ColorPalette>(
                m_voxelWorld->drawContext(), models[0].palette);

            auto voxelData = std::make_shared<VoxelObjectVoxelData>(
                *m_voxelWorld, palette, models[0].size);
            voxelData->addVoxelsRaw(models[0].voxels);
            voxelObject.setVoxelData(voxelData);

            const auto iter = m_newJson.find("CrucialVoxel");
            if (iter != m_newJson.end())
            {
                glm::uvec3 crucialVoxel = *iter;
                std::swap(crucialVoxel.y, crucialVoxel.z);
                voxelObject.setCrucialVoxel(crucialVoxel);
            }
        }
    }
}
