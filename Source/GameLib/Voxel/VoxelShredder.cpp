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

std::vector<std::vector<glm::uvec3>> VoxelShredder::shred(const std::vector<glm::uvec3> & input,
                                             const glm::uvec3 & llf,
                                             const glm::uvec3 & urb,
                                                          u32 depth
)
{
    const auto inputCenter = (glm::vec3(llf) + glm::vec3(urb)) / 2.0f;

    const auto separationPlaneNormal = RandomUnitVec3();
    const auto separationPlaneD = glm::length(inputCenter);

    std::vector<glm::uvec3> left;
    std::vector<glm::uvec3> right;

    auto leftLlf = glm::uvec3(std::numeric_limits<u32>::max());
    auto leftUrb = glm::uvec3(0);
    auto rightLlf = leftLlf;
    auto rightUrb = leftUrb;

    for (const auto & cell : input) {
        const auto position = glm::vec3(cell);

        if (glm::dot(position, separationPlaneNormal) > separationPlaneD) {
            leftLlf = std::min(cell, leftLlf);
            leftUrb = std::max(cell, leftUrb);
            left.emplace_back(cell);
        }
        else {
            rightLlf = std::min(cell, rightLlf);
            rightUrb = std::max(cell, rightUrb);
            right.emplace_back(cell);
        }
    }

    if (depth <= m_minShredDepth)
    {
        auto leftShredded = shred(left, leftLlf, leftUrb, depth + 1);
        auto rightShredded = shred(right, rightLlf, rightUrb, depth + 1);

        std::vector result;
        result.resize(leftShredded.size() + rightShredded.size());
        std::copy(leftShredded.begin(), leftShredded.end(), result.begin());
        std::copy(rightShredded.begin(), rightShredded.end(), result.begin() + leftShredded.size());

        return result;
    }
    else
    {
        std::vector result;

        if (!left.empty()) result.emplace_back(left);
        if (!right.empty()) result.emplace_back(right);

        return result;
    }

}