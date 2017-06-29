#include "VoxelObjectPrototype.h"

#include "ColorPalette.h"
#include "VoxReader.h"
#include "VoxelObject.h"
#include "VoxelObjectVoxelData.h"

VoxelObjectPrototype::VoxelObjectPrototype(const std::shared_ptr<VoxelWorld> & voxelWorld):
    m_voxelWorld(voxelWorld) {
}

void VoxelObjectPrototype::initComponent(VoxelObject & voxelObject)
{
    const auto voxelClusterName = m_json["VoxelCluster"].get<std::string>();

    VoxReader voxReader;
    {
        auto models = voxReader.read(GameDataPath("Data/VoxelClusters/" + voxelClusterName + ".vox"));
        if (!models.empty())
        {
            auto palette = std::make_shared<ColorPalette>(m_voxelWorld->drawContext(),
                                                          models[0].palette);

            auto voxelData = std::make_shared<VoxelObjectVoxelData>(*m_voxelWorld,
                                                                          palette,
                                                                          models[0].size);
            voxelData->addVoxelsRaw(models[0].voxels);
            voxelObject.setVoxelData(voxelData);

            const auto iter = m_json.find("CrucialVoxel");
            if (iter != m_json.end())
            {
                glm::uvec3 crucialVoxel = *iter;
                voxelObject.setCrucialVoxel(crucialVoxel);
            }
        }
    }

    {
        auto iter = m_json.find("Scale");
        if (iter != m_json.end()) voxelObject.setScale(*iter);
    }
}
