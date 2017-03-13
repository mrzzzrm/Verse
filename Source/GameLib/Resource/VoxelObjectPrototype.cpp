#include "VoxelObjectPrototype.h"

#include "VoxReader.h"
#include "VoxelObject.h"
#include "VoxelObjectVoxelData.h"

VoxelObjectPrototype::VoxelObjectPrototype(const Json & json, VoxelWorld & voxelWorld)
{
    const auto & voxelClusterName = json["VoxelCluster"].get<std::string>();

    VoxReader voxReader;
    {
        auto models = voxReader.read("Data/VoxelClusters/" + voxelClusterName + ".vox");
        if (!models.empty())
        {
            m_voxelDataPrototype = std::make_shared<VoxelObjectVoxelData>(voxelWorld, models[0].size);
            m_voxelDataPrototype->addVoxels(models[0].voxels);
            m_voxelDataPrototype->setCrucialVoxel({11, 12, 6}); // TODO
        }
    }

    m_scale = json["Scale"];
}

void VoxelObjectPrototype::applyToEntity(Entity & entity) const
{
    auto & voxelObject = entity.addComponent<VoxelObject>(*m_voxelDataPrototype);
    voxelObject.setScale(m_scale);
}