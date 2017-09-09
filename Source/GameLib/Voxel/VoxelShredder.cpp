#include "VoxelShredder.h"

#include <Deliberation/Core/Math/Random.h>

#include <glm/glm.hpp>

VoxelShredder::VoxelShredder(const VoxelCluster<u32> & inputCluster):
    m_inputCluster(inputCluster)
{

}

std::vector<VoxelCluster<u32>> VoxelShredder::process()
{
    std::vector<glm::uvec3> voxels;
    voxels.reserve(m_inputCluster.numVoxels());

    for (size_t v = 0; v < m_inputCluster.voxels().size(); ++v)
    {
        if (m_inputCluster.test(v)) voxels.emplace_back(m_inputCluster.indexToVoxel(v));
    }

    auto segments = shred(voxels, {0, 0, 0}, m_inputCluster.size(), 0);
}

std::vector<glm::uvec3> VoxelShredder::shred(const std::vector<glm::uvec3> & input,
                                             const glm::uvec3 & llf,
                                             const glm::uvec3 & urb,
)
{
    const auto inputCenter = (glm::vec3(llf) + glm::vec3(urb)) / 2.0f;

    const auto separationPlaneNormal = RandomUnitVec3();
    const auto separationPlaneD = glm::length(inputCenter);

    std::vector<glm::uvec3> left;
    std::vector<glm::uvec3> right;

    for (const auto & cell : input) {
        const auto position = glm::vec3(cell);

        if (glm::dot(position, separationPlaneNormal) > separationPlaneD) left.emplace_back(cell);
        else right.emplace_back(cell);
    }


}