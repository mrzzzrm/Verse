#include "VoxelClusterMarchingCubesTriangulation.h"

#include <algorithm>
#include <iostream>

using namespace deliberation;

namespace
{
const std::array<VoxelClusterMarchingCubesConfig, 18> BASE_CONFIGS = {
    0b00000000, // 0
    0b00001000, // 1
    0b01001000, // 2
    0b00001001, // 3
    0b00101000, // 4
    0b00001110, // 5
    0b01001001, // 6
    0b01011000, // 7
    0b00011110, // 8
    0b01101001, // 9
    0b01011010, // 10
    0b00001111, // 11
    0b10001110, // 12
    0b01001101, // 13
    0b01001110, // 14
    0b11011110, // 15
    0b10011110, // 16
    0b01011110, // 17
};

const std::array<std::vector<uint>, 18> BASE_MESHES = {
    std::vector<uint>(),                                                 // 0
    std::vector<uint>{{3, 7, 2}},                                        // 1
    std::vector<uint>{{3, 7, 2, 10, 9, 6}},                              // 2
    std::vector<uint>{{0, 4, 7, 0, 7, 2}},                               // 3
    std::vector<uint>{{3, 7, 2, 8, 5, 9}},                               // 4
    std::vector<uint>{{0, 3, 5, 3, 7, 5, 5, 7, 6}},                      // 5
    std::vector<uint>{{0, 4, 7, 0, 7, 2, 10, 9, 6}},                     // 6
    std::vector<uint>{{3, 7, 2, 10, 9, 6, 11, 4, 8}},                    // 7
    std::vector<uint>{{0, 3, 5, 3, 7, 5, 5, 7, 6, 11, 4, 8}},            // 8
    std::vector<uint>{{0, 4, 7, 0, 7, 2, 5, 10, 8, 5, 6, 10}},           // 9
    std::vector<uint>{{3, 7, 2, 10, 9, 6, 11, 4, 8, 0, 1, 5}},           // 10
    std::vector<uint>{{4, 7, 5, 5, 7, 6}},                               // 11
    std::vector<uint>{{3, 11, 0, 0, 11, 5, 5, 11, 10, 5, 10, 6}},        // 12
    std::vector<uint>{{10, 4, 7, 4, 10, 9, 4, 9, 0, 0, 9, 1}},           // 13
    std::vector<uint>{{3, 7, 10, 3, 10, 9, 3, 9, 0, 0, 9, 5}},           // 14
    std::vector<uint>{{4, 8, 3, 3, 8, 9, 3, 9, 5, 3, 5, 0}},             // 15
    std::vector<uint>{{4, 8, 3, 8, 10, 3, 3, 10, 6, 3, 6, 5, 3, 5, 0}},  // 16
    std::vector<uint>{{3, 7, 10, 3, 10, 9, 3, 9, 0, 0, 9, 5, 4, 8, 11}}, // 17
};

const std::array<glm::vec3, 12> BASE_EDGE_POSITIONS = {{
    {0.0f, -0.5f, -0.5f},
    {0.5f, -0.5f, 0.0f},
    {0.0f, -0.5f, 0.5f},
    {-0.5f, -0.5f, 0.0f},
    {-0.5f, 0.0f, -0.5f},
    {0.5f, 0.0f, -0.5f},
    {0.5f, 0.0f, 0.5f},
    {-0.5f, 0.0f, 0.5f},
    {0.0f, 0.5f, -0.5f},
    {0.5f, 0.5f, 0.0f},
    {0.0f, 0.5f, 0.5f},
    {-0.5f, 0.5f, 0.0f},
}};

const std::array<
    std::pair<VoxelClusterMarchingCubesCorner, VoxelClusterMarchingCubesCorner>,
    12>
    BASE_EDGE_CORNERS = {{
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 0},
    }};

const std::array<glm::vec3, 8> CORNER_POSITIONS = {{
    {-0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, 0.5f},
    {-0.5f, -0.5f, 0.5f},
    {-0.5f, 0.5f, -0.5f},
    {0.5f, 0.5f, -0.5f},
    {0.5f, 0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f},
}};

struct BaseTriangleIndex
{
    BaseTriangleIndex(u8 a) : a(a) {}

    BaseTriangleIndex(u8 a, u8 b) : a(a), b(b) {}

    BaseTriangleIndex(u8 a, u8 b, u8 c) : a(a), b(b), c(c) {}

    void rotate(uint x, uint y, uint z)
    {
        Assert(a != VoxelClusterMarchingCubesTriangulation::INVALID_CORNER);

        a = VoxelClusterMarchingCubesTriangulation::rotateEdge(a, x, y, z);

        if (b != VoxelClusterMarchingCubesTriangulation::INVALID_CORNER)
        {
            b = VoxelClusterMarchingCubesTriangulation::rotateEdge(b, x, y, z);
        }
        if (c != VoxelClusterMarchingCubesTriangulation::INVALID_CORNER)
        {
            c = VoxelClusterMarchingCubesTriangulation::rotateEdge(c, x, y, z);
        }
    }

    glm::vec3 toPosition() const
    {
        Assert(a != VoxelClusterMarchingCubesTriangulation::INVALID_CORNER);

        if (b != VoxelClusterMarchingCubesTriangulation::INVALID_CORNER)
        {
            if (c != VoxelClusterMarchingCubesTriangulation::INVALID_CORNER)
            {
                return (BASE_EDGE_POSITIONS[a] + BASE_EDGE_POSITIONS[b] +
                        BASE_EDGE_POSITIONS[c]) /
                       3.0f;
            }
            else
            {
                return (BASE_EDGE_POSITIONS[a] + BASE_EDGE_POSITIONS[b]) / 2.0f;
            }
        }
        else
        {
            return BASE_EDGE_POSITIONS[a];
        }
    }

    u8 a = VoxelClusterMarchingCubesTriangulation::INVALID_CORNER;
    u8 b = VoxelClusterMarchingCubesTriangulation::INVALID_CORNER;
    u8 c = VoxelClusterMarchingCubesTriangulation::INVALID_CORNER;
};

struct BaseTriangle
{
    BaseTriangle(
        u8                corner,
        BaseTriangleIndex a,
        BaseTriangleIndex b,
        BaseTriangleIndex c)
        : corner(corner), a(a), b(b), c(c)
    {
    }

    void rotate(uint x, uint y, uint z)
    {
        corner = VoxelClusterMarchingCubesTriangulation::rotateCorner(
            corner, x, y, z);
        a.rotate(x, y, z);
        b.rotate(x, y, z);
        c.rotate(x, y, z);
    }

    VoxelClusterMarchingCubesTriangulation::Triangle build() const
    {
        VoxelClusterMarchingCubesTriangulation::Triangle result;
        result.corner = corner;
        result.positions[0] = a.toPosition();
        result.positions[1] = b.toPosition();
        result.positions[2] = c.toPosition();

        auto a = result.positions[0] - result.positions[2];
        auto b = result.positions[1] - result.positions[2];
        result.normal = glm::normalize(glm::cross(a, b));

        return result;
    }

    u8                corner;
    BaseTriangleIndex a;
    BaseTriangleIndex b;
    BaseTriangleIndex c;
};

struct BaseMesh
{
    BaseMesh() = default;

    BaseMesh(const std::vector<BaseTriangle> & triangles) : triangles(triangles)
    {
    }

    void addFace(u8 corner, const std::vector<BaseTriangleIndex> & indices)
    {
        for (uint i = 1; i < indices.size() - 1; i++)
        {
            triangles.emplace_back(
                corner, indices[0], indices[i], indices[i + 1]);
        }
    }

    std::vector<BaseTriangle> triangles;
};

} // namespace

VoxelClusterMarchingCubesConfig
VoxelClusterMarchingCubesTriangulation::rotateConfigX(
    VoxelClusterMarchingCubesConfig config)
{
    VoxelClusterMarchingCubesConfig result;
    result.set(0, config[4]);
    result.set(1, config[5]);
    result.set(2, config[1]);
    result.set(3, config[0]);
    result.set(4, config[7]);
    result.set(5, config[6]);
    result.set(6, config[2]);
    result.set(7, config[3]);
    return result;
}

VoxelClusterMarchingCubesConfig
VoxelClusterMarchingCubesTriangulation::rotateConfigY(
    VoxelClusterMarchingCubesConfig config)
{
    VoxelClusterMarchingCubesConfig result;
    result.set(0, config[3]);
    result.set(1, config[0]);
    result.set(2, config[1]);
    result.set(3, config[2]);
    result.set(4, config[7]);
    result.set(5, config[4]);
    result.set(6, config[5]);
    result.set(7, config[6]);
    return result;
}

VoxelClusterMarchingCubesConfig
VoxelClusterMarchingCubesTriangulation::rotateConfigZ(
    VoxelClusterMarchingCubesConfig config)
{
    VoxelClusterMarchingCubesConfig result;
    result.set(0, config[1]);
    result.set(1, config[5]);
    result.set(2, config[6]);
    result.set(3, config[2]);
    result.set(4, config[0]);
    result.set(5, config[4]);
    result.set(6, config[7]);
    result.set(7, config[3]);
    return result;
}

u8 VoxelClusterMarchingCubesTriangulation::rotateCorner(
    u8 corner, uint x, uint y, uint z)
{
    static u8 X[] = {3, 2, 6, 7, 0, 1, 5, 4};
    static u8 Y[] = {1, 2, 3, 0, 5, 6, 7, 4};
    static u8 Z[] = {4, 0, 3, 7, 5, 1, 2, 6};

    for (uint i = 0; i < x; i++)
        corner = X[corner];
    for (uint i = 0; i < y; i++)
        corner = Y[corner];
    for (uint i = 0; i < z; i++)
        corner = Z[corner];

    return corner;
}

u8 VoxelClusterMarchingCubesTriangulation::rotateEdge(
    u8 edge, uint x, uint y, uint z)
{
    static u8 X[] = {2, 6, 10, 7, 3, 1, 9, 11, 0, 5, 8, 4};
    static u8 Y[] = {1, 2, 3, 0, 5, 6, 7, 4, 9, 10, 11, 8};
    static u8 Z[] = {4, 3, 7, 11, 8, 0, 2, 10, 5, 1, 6, 9};

    for (uint i = 0; i < x; i++)
        edge = X[edge];
    for (uint i = 0; i < y; i++)
        edge = Y[edge];
    for (uint i = 0; i < z; i++)
        edge = Z[edge];

    return edge;
}

VoxelClusterMarchingCubesTriangulation::VoxelClusterMarchingCubesTriangulation()
{
    /**
     * Setup base meshes
     */
    std::array<BaseMesh, NUMBER_OF_BASE_AND_INVERSE_CONFIGS> baseMeshes;

    baseMeshes[0] = {};

    baseMeshes[1] = {{{3, {3}, {7}, {2}}}};

    baseMeshes[19].addFace(
        0, std::vector<BaseTriangleIndex>({{3}, {3, 2}, {7, 3, 2}, {3, 7}}));
    baseMeshes[19].addFace(
        2, std::vector<BaseTriangleIndex>({{3, 2}, {2}, {7, 2}, {7, 3, 2}}));
    baseMeshes[19].addFace(
        7, std::vector<BaseTriangleIndex>({{3, 7}, {7, 3, 2}, {7, 2}, {7}}));

    baseMeshes[2] = {{
        {3, {3}, {7}, {2}},
        {6, {10}, {9}, {6}},
    }};

    baseMeshes[3].addFace(
        0, std::vector<BaseTriangleIndex>({{4}, {4, 7}, {0, 2}, {0}}));
    baseMeshes[3].addFace(
        3, std::vector<BaseTriangleIndex>({{4, 7}, {7}, {2}, {0, 2}}));

    baseMeshes[21].addFace(
        4, std::vector<BaseTriangleIndex>({{4}, {4, 0}, {4, 2}, {4, 7}}));
    baseMeshes[21].addFace(
        1, std::vector<BaseTriangleIndex>({{0, 4}, {0}, {0, 2}, {4, 2}}));
    baseMeshes[21].addFace(
        7, std::vector<BaseTriangleIndex>({{4, 7}, {4, 2}, {7, 2}, {7}}));
    baseMeshes[21].addFace(
        2, std::vector<BaseTriangleIndex>({{4, 2}, {0, 2}, {2}, {7, 2}}));

    baseMeshes[4] = {{{3, {3}, {7}, {2}}, {5, {9}, {8}, {5}}}};

    baseMeshes[22].addFace(
        0, std::vector<BaseTriangleIndex>({{3}, {3, 2}, {7, 3, 2}, {3, 7}}));
    baseMeshes[22].addFace(
        2, std::vector<BaseTriangleIndex>({{3, 2}, {2}, {7, 2}, {7, 3, 2}}));
    baseMeshes[22].addFace(
        7, std::vector<BaseTriangleIndex>({{3, 7}, {7, 3, 2}, {7, 2}, {7}}));
    baseMeshes[22].addFace(
        4, std::vector<BaseTriangleIndex>({{8}, {8, 9}, {8, 9, 5}, {8, 5}}));
    baseMeshes[22].addFace(
        6, std::vector<BaseTriangleIndex>({{9}, {5, 9}, {8, 9, 5}, {8, 9}}));
    baseMeshes[22].addFace(
        1, std::vector<BaseTriangleIndex>({{5}, {5, 8}, {8, 9, 5}, {5, 9}}));

    baseMeshes[5].addFace(
        3, std::vector<BaseTriangleIndex>({{3, 0}, {3}, {7}, {7, 5}}));
    baseMeshes[5].addFace(
        3, std::vector<BaseTriangleIndex>({{7}, {7, 6}, {7, 5}}));
    baseMeshes[5].addFace(
        2, std::vector<BaseTriangleIndex>({{5, 6}, {4, 6}, {7, 6}, {6}}));
    baseMeshes[5].addFace(
        1, std::vector<BaseTriangleIndex>({{0}, {0, 3}, {7, 5}, {5}}));
    baseMeshes[5].addFace(
        1, std::vector<BaseTriangleIndex>({{5}, {7, 5}, {5, 6}}));

    baseMeshes[23].addFace(
        0, std::vector<BaseTriangleIndex>({{3}, {0}, {0, 5}, {5, 7}, {3, 7}}));
    baseMeshes[23].addFace(
        5, std::vector<BaseTriangleIndex>({{0, 5}, {5}, {5, 7}}));
    baseMeshes[23].addFace(
        5, std::vector<BaseTriangleIndex>({{5}, {5, 6}, {5, 7}}));
    baseMeshes[23].addFace(
        6, std::vector<BaseTriangleIndex>({{5, 6}, {6}, {6, 7}, {5, 7}}));
    baseMeshes[23].addFace(
        7, std::vector<BaseTriangleIndex>({{3, 7}, {7, 5}, {7}}));
    baseMeshes[23].addFace(
        7, std::vector<BaseTriangleIndex>({{5, 7}, {7, 6}, {7}}));

    baseMeshes[6].addFace(
        0, std::vector<BaseTriangleIndex>({{4}, {4, 7}, {0, 2}, {0}}));
    baseMeshes[6].addFace(
        3, std::vector<BaseTriangleIndex>({{4, 7}, {7}, {2}, {0, 2}}));
    baseMeshes[6].addFace(6, std::vector<BaseTriangleIndex>({{10}, {9}, {6}}));

    baseMeshes[7] = {{
        {3, {3}, {7}, {2}},
        {6, {10}, {9}, {6}},
        {4, {8}, {11}, {4}},
    }};

    baseMeshes[8] = baseMeshes[5];
    baseMeshes[8].addFace(4, std::vector<BaseTriangleIndex>({{8}, {11}, {4}}));

    baseMeshes[9] = baseMeshes[3];
    baseMeshes[9].addFace(
        5, std::vector<BaseTriangleIndex>({{5}, {5, 6}, {8, 10}, {8}}));
    baseMeshes[9].addFace(
        6, std::vector<BaseTriangleIndex>({{5, 6}, {6}, {10}, {8, 10}}));

    baseMeshes[10] = baseMeshes[7];
    baseMeshes[10].addFace(1, std::vector<BaseTriangleIndex>({{0}, {1}, {5}}));

    baseMeshes[11].addFace(
        0, std::vector<BaseTriangleIndex>({{4}, {4, 7}, {7, 5}, {4, 5}}));
    baseMeshes[11].addFace(
        1, std::vector<BaseTriangleIndex>({{5}, {4, 5}, {7, 5}, {6, 5}}));
    baseMeshes[11].addFace(
        2, std::vector<BaseTriangleIndex>({{6}, {6, 5}, {7, 5}, {6, 7}}));
    baseMeshes[11].addFace(
        3, std::vector<BaseTriangleIndex>({{7}, {6, 7}, {7, 5}, {4, 7}}));

    baseMeshes[12].addFace(
        1, std::vector<BaseTriangleIndex>({{0, 3}, {11, 0}, {0}}));
    baseMeshes[12].addFace(
        1, std::vector<BaseTriangleIndex>({{0}, {11, 0}, {11, 5}, {5}}));
    baseMeshes[12].addFace(
        1, std::vector<BaseTriangleIndex>({{5}, {11, 5}, {10, 5}}));
    baseMeshes[12].addFace(
        1, std::vector<BaseTriangleIndex>({{5, 10}, {5, 6}, {5}}));
    baseMeshes[12].addFace(
        2, std::vector<BaseTriangleIndex>({{5, 6}, {5, 10}, {6, 10}, {6}}));
    baseMeshes[12].addFace(
        3, std::vector<BaseTriangleIndex>({{11, 3}, {11, 0}, {3, 0}, {3}}));
    baseMeshes[12].addFace(
        7, std::vector<BaseTriangleIndex>({{11, 3}, {11}, {11, 0}}));
    baseMeshes[12].addFace(
        7, std::vector<BaseTriangleIndex>({{11, 0}, {11}, {11, 5}}));
    baseMeshes[12].addFace(
        7, std::vector<BaseTriangleIndex>({{10, 5}, {10}, {10, 6}}));
    baseMeshes[12].addFace(
        7, std::vector<BaseTriangleIndex>({{11, 5}, {11}, {10}, {10, 5}}));

    baseMeshes[13].addFace(
        0, std::vector<BaseTriangleIndex>({{4, 7}, {4, 10}, {4}}));
    baseMeshes[13].addFace(
        0, std::vector<BaseTriangleIndex>({{4}, {4, 10}, {4, 9}}));
    baseMeshes[13].addFace(
        0, std::vector<BaseTriangleIndex>({{4}, {4, 9}, {0, 9}, {0}}));
    baseMeshes[13].addFace(
        0, std::vector<BaseTriangleIndex>({{0}, {0, 9}, {0, 1}}));

    baseMeshes[13].addFace(
        2, std::vector<BaseTriangleIndex>({{0, 1}, {0, 9}, {9, 1}, {1}}));
    baseMeshes[13].addFace(
        3, std::vector<BaseTriangleIndex>({{4, 7}, {7}, {7, 10}, {10, 4}}));

    baseMeshes[13].addFace(
        6, std::vector<BaseTriangleIndex>({{4, 10}, {7, 10}, {10}}));
    baseMeshes[13].addFace(
        6, std::vector<BaseTriangleIndex>({{4, 10}, {10}, {9}, {4, 9}}));
    baseMeshes[13].addFace(
        6, std::vector<BaseTriangleIndex>({{4, 9}, {9}, {9, 0}}));
    baseMeshes[13].addFace(
        6, std::vector<BaseTriangleIndex>({{1, 9}, {0, 9}, {9}}));

    baseMeshes[14].addFace(
        1, std::vector<BaseTriangleIndex>({{3, 0}, {7, 5}, {9, 5}, {5}, {0}}));
    baseMeshes[14].addFace(
        3, std::vector<BaseTriangleIndex>({{3, 0}, {3}, {7}, {7, 10}, {5, 7}}));
    baseMeshes[14].addFace(
        6,
        std::vector<BaseTriangleIndex>({{10, 7}, {10}, {9}, {9, 5}, {7, 5}}));

    baseMeshes[15].addFace(
        1, std::vector<BaseTriangleIndex>({{0}, {0, 3}, {3, 5}, {5}}));
    baseMeshes[15].addFace(
        1, std::vector<BaseTriangleIndex>({{5}, {5, 3}, {3, 9}, {5, 9}}));
    baseMeshes[15].addFace(
        3, std::vector<BaseTriangleIndex>({{3}, {3, 4}, {3, 8}}));
    baseMeshes[15].addFace(
        3, std::vector<BaseTriangleIndex>({{3}, {3, 8}, {3, 9}}));
    baseMeshes[15].addFace(
        3, std::vector<BaseTriangleIndex>({{0, 3}, {3}, {3, 5}}));
    baseMeshes[15].addFace(
        3, std::vector<BaseTriangleIndex>({{5, 3}, {3}, {3, 9}}));
    baseMeshes[15].addFace(
        4, std::vector<BaseTriangleIndex>({{4, 3}, {4}, {8}, {8, 3}}));
    baseMeshes[15].addFace(
        4, std::vector<BaseTriangleIndex>({{8}, {8, 9}, {3, 9}, {8, 3}}));
    baseMeshes[15].addFace(
        6, std::vector<BaseTriangleIndex>({{3, 9}, {8, 9}, {9}}));
    baseMeshes[15].addFace(
        6, std::vector<BaseTriangleIndex>({{3, 9}, {9}, {5, 9}}));

    baseMeshes[16].addFace(
        1, std::vector<BaseTriangleIndex>({{0}, {0, 3}, {3, 5}, {5}}));
    baseMeshes[16].addFace(
        1, std::vector<BaseTriangleIndex>({{3, 5}, {3, 5, 6}, {6, 5}, {5}}));
    baseMeshes[16].addFace(
        2, std::vector<BaseTriangleIndex>({{3, 6, 10}, {6, 10}, {6}, {3, 6}}));
    baseMeshes[16].addFace(
        2, std::vector<BaseTriangleIndex>({{3, 6}, {6}, {6, 5}, {3, 5, 6}}));
    baseMeshes[16].addFace(
        3, std::vector<BaseTriangleIndex>({{3, 4}, {3, 8}, {3}}));
    baseMeshes[16].addFace(
        3, std::vector<BaseTriangleIndex>({{3}, {3, 8}, {3, 8, 10}, {3, 10}}));
    baseMeshes[16].addFace(
        3, std::vector<BaseTriangleIndex>({{3}, {3, 10}, {3, 10, 6}, {3, 6}}));
    baseMeshes[16].addFace(
        3, std::vector<BaseTriangleIndex>({{3}, {3, 6}, {3, 6, 5}, {3, 5}}));
    baseMeshes[16].addFace(
        3, std::vector<BaseTriangleIndex>({{3}, {3, 5}, {3, 0}}));
    baseMeshes[16].addFace(
        4, std::vector<BaseTriangleIndex>({{3, 4}, {4}, {8}, {3, 8}}));
    baseMeshes[16].addFace(
        4, std::vector<BaseTriangleIndex>({{3, 8}, {8}, {8, 10}, {3, 8, 10}}));
    baseMeshes[16].addFace(
        7,
        std::vector<BaseTriangleIndex>({{10, 8}, {10}, {3, 10}, {3, 8, 10}}));
    baseMeshes[16].addFace(
        7,
        std::vector<BaseTriangleIndex>({{10}, {10, 6}, {3, 6, 10}, {3, 10}}));

    baseMeshes[17] = baseMeshes[14];
    baseMeshes[17].addFace(4, std::vector<BaseTriangleIndex>({{8}, {11}, {4}}));

    /**
     * Generate inverse configs
     */
    std::array<
        VoxelClusterMarchingCubesConfig,
        NUMBER_OF_BASE_AND_INVERSE_CONFIGS>
        baseAndInverseConfigs;
    std::copy(
        BASE_CONFIGS.begin(),
        BASE_CONFIGS.end(),
        baseAndInverseConfigs.begin());
    for (uint c = (uint)BASE_CONFIGS.size(); c < baseAndInverseConfigs.size();
         c++)
    {
        baseAndInverseConfigs[c] = ~BASE_CONFIGS[c - BASE_CONFIGS.size()];
    }

    /**
     * Rotate base meshes into configs
     */
    std::array<bool, NUMBER_OF_CONFIGS> configSet;
    std::generate(configSet.begin(), configSet.end(), [] { return false; });

    for (size_t c = 0; c < baseMeshes.size(); c++)
    {
        auto & baseMesh = baseMeshes[c];
        auto & originalConfig = baseAndInverseConfigs[c];

        VoxelClusterMarchingCubesConfig baseClassX = originalConfig;
        for (uint x = 0; x < 4; x++)
        {
            VoxelClusterMarchingCubesConfig baseClassY = baseClassX;
            for (uint y = 0; y < 4; y++)
            {
                VoxelClusterMarchingCubesConfig baseClassZ = baseClassY;
                for (uint z = 0; z < 4; z++)
                {
                    auto configID = baseClassZ.to_ullong();

                    if (!configSet[configID])
                    {
                        configSet[configID] = true;

                        auto & config = m_configs[configID];

                        for (auto & originalTriangle : baseMesh.triangles)
                        {
                            auto triangle = originalTriangle;
                            triangle.rotate(x, y, z);

                            config.emplace_back(triangle.build());
                        }
                    }

                    baseClassZ = rotateConfigZ(baseClassZ);
                }

                baseClassY = rotateConfigY(baseClassY);
            }

            baseClassX = rotateConfigX(baseClassX);
        }
    }
}

const VoxelClusterMarchingCubesTriangulation::Configs &
VoxelClusterMarchingCubesTriangulation::configs() const
{
    return m_configs;
}
