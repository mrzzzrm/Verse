#include "VoxelObjectPrototype.h"

#include "VoxReader.h"
#include "VoxelObject.h"
#include "VoxelObjectVoxelData.h"

VoxelObjectPrototype::VoxelObjectPrototype(const Json & json, VoxelWorld & voxelWorld)
{
    const auto & voxelClusterName = json["VoxelCluster"].get<std::string>();

    VoxReader voxReader;
    {
        auto models = voxReader.read(GameDataPath("Data/VoxelClusters/" + voxelClusterName + ".vox"));
        if (!models.empty())
        {
            m_voxelDataPrototype = std::make_shared<VoxelObjectVoxelData>(voxelWorld, models[0].size);
            m_voxelDataPrototype->addVoxels(models[0].voxels);

            const auto iter = json.find("CrucialVoxel");
            if (iter != json.end())
            {
                glm::uvec3 crucialVoxel = *iter;
                m_crucialVoxel = crucialVoxel;
            }
        }
    }

    {
        auto iter = json.find("Scale");
        if (iter != json.end()) m_scale = *iter;
    }
}

void VoxelObjectPrototype::applyToEntity(Entity & entity) const
{
    auto & voxelObject = entity.addComponent<VoxelObject>(*m_voxelDataPrototype);
    voxelObject.setScale(m_scale);
    voxelObject.setCrucialVoxel(m_crucialVoxel);
}