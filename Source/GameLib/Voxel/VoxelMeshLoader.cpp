#include "VoxelMeshLoader.h"

#include "Deliberation/Resource/ResourceManager.h"
#include "VoxelWorld.h"
#include "VoxReader.h"
#include "VoxelClusterMarchingCubes.h"
#include "VoxelCluster.h"

std::shared_ptr<MeshData> VoxelMeshLoader::load(const std::string & path)
{
    auto & voxelModels = App::get().runtime()->resourceManager()->resource<VoxReader::VoxelModels>(path);
    AssertM(voxelModels.size() == 1, "Wrong number of models in '" + path + "'");

    auto & voxelModel = voxelModels[0];

    VoxelCluster<u32> cluster(voxelModel.size);
    for (const auto & voxel : voxelModel.voxels)
    {
        cluster.set(voxel.cell, voxel.colorIndex);
    }

    VoxelClusterMarchingCubes marchingCubes(App::get().runtime()->world()->systemRef<VoxelWorld>().marchingCubesTriangulation(),
        cluster);
    marchingCubes.run();

    auto palettedVertices = marchingCubes.takeVertices();

    auto layout = DataLayout({{"Position", Type_Vec3},
                              {"Normal", Type_Vec3},
                              {"Color", Type_U8Vec4}});

    LayoutedBlob vertices(layout, palettedVertices.count());

    auto outPositions = vertices.iterator<glm::vec3>("Position");
    auto outNormals = vertices.iterator<glm::vec3>("Normal");
    auto outColors = vertices.iterator<glm::u8vec4>("Color");

    auto inPositions = palettedVertices.iterator<glm::vec3>("Position");
    auto inNormals = palettedVertices.iterator<glm::vec3>("Normal");
    auto inColorIndices = palettedVertices.iterator<u32>("ColorIndex");

    for (size_t i = 0; i < palettedVertices.count(); i++)
    {
        outPositions.put(inPositions.get());
        outNormals.put(inNormals.get());

        auto colorIndex = inColorIndices.get();
        outColors.put(voxelModel.palette[colorIndex]);
    }

    return std::make_shared<MeshData>(std::move(vertices), LayoutedBlob());

}