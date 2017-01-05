#pragma once

#include <array>
#include <bitset>
#include <vector>

#include <glm/vec3.hpp>

#include <Deliberation/Deliberation.h>

#include "VoxelCluster.h"

#include "GameLib.h"

using VoxelClusterMarchingCubesConfig = std::bitset<8>;
using VoxelClusterMarchingCubesCorner = u8;

class VoxelClusterMarchingCubesTriangulation final
{
public:
    static constexpr uint NUMBER_OF_CONFIGS = 256;
    static constexpr u8 INVALID_CORNER = 255;
    static constexpr u8 NUMBER_OF_BASE_CONFIGS = 18;
    static constexpr u8 NUMBER_OF_BASE_AND_INVERSE_CONFIGS = NUMBER_OF_BASE_CONFIGS * 2;

public:
    static VoxelClusterMarchingCubesConfig rotateConfigX(VoxelClusterMarchingCubesConfig config);
    static VoxelClusterMarchingCubesConfig rotateConfigY(VoxelClusterMarchingCubesConfig config);
    static VoxelClusterMarchingCubesConfig rotateConfigZ(VoxelClusterMarchingCubesConfig config);
    static u8 rotateCorner(u8 corner, uint x, uint y, uint z);
    static u8 rotateEdge(u8 edge, uint x, uint y, uint z);

public:
    struct Triangle
    {
        std::array<glm::vec3, 3> positions;
        glm::vec3 normal;
        u8 corner = INVALID_CORNER;
    };

    using Triangulation = std::vector<Triangle>;
    using Configs = std::array<Triangulation, NUMBER_OF_CONFIGS>;

public:
    VoxelClusterMarchingCubesTriangulation();

    const Configs & configs() const;

private:
    Configs m_configs;
};