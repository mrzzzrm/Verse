#include <Deliberation/Core/Math/PrimitiveCompare.h>

#include <Deliberation/Core/Math/Sphere.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include "VoxelCluster.h"
#include "VoxelClusterShape.h"

#include <memory>

#include "gtest/gtest.h"

class VoxelClusterShapeTest : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        cluster1x1x1 = VoxelCluster<bool>({1, 1, 1});
        cluster1x1x1.set({0, 0, 0}, true);
        shape1x1x1 = VoxelClusterShape(cluster1x1x1);

        cluster2x1x1 = VoxelCluster<bool>({2, 1, 1});
        cluster2x1x1.set({0, 0, 0}, true);
        cluster2x1x1.set({1, 0, 0}, true);
        shape2x1x1 = VoxelClusterShape(cluster2x1x1);

        cluster3x2x1 = VoxelCluster<bool>({3, 2, 1});
        cluster3x2x1.set({0, 0, 0}, true);
        cluster3x2x1.set({1, 0, 0}, true);
        cluster3x2x1.set({2, 0, 0}, true);
        cluster3x2x1.set({0, 1, 0}, true);
        shape3x2x1 = VoxelClusterShape(cluster3x2x1);
    }

    VoxelCluster<bool> cluster1x1x1;
    VoxelClusterShape shape1x1x1;
    VoxelCluster<bool> cluster2x1x1;
    VoxelClusterShape shape2x1x1;
    VoxelCluster<bool> cluster3x2x1;
    VoxelClusterShape shape3x2x1;
};

TEST_F(VoxelClusterShapeTest, Basic)
{
    ASSERT_TRUE(shape1x1x1.size() == glm::uvec3(1, 1, 1));
    ASSERT_TRUE(shape1x1x1.m_nodeMask == std::vector<bool>({true}));
    ASSERT_TRUE(shape1x1x1.m_nodes[0].leaf.voxel == glm::uvec3(0, 0, 0));


    ASSERT_TRUE(shape2x1x1.size() == glm::uvec3(2, 1, 1));
    ASSERT_TRUE(shape2x1x1.m_nodeMask == std::vector<bool>({true, true, true}));
    ASSERT_TRUE(PrimitivesEqual(shape2x1x1.m_nodes[0].node.bounds, Sphere(glm::vec3(1.0f, 0.5f, 0.5f), 1.22f), 0.01f));
    ASSERT_TRUE(shape2x1x1.m_nodes[1].leaf.voxel == glm::uvec3(0, 0, 0));
    ASSERT_TRUE(shape2x1x1.m_nodes[2].leaf.voxel == glm::uvec3(1, 0, 0));

    ASSERT_TRUE(shape3x2x1.size() == glm::uvec3(3, 2, 1));
    ASSERT_TRUE(shape3x2x1.m_nodeMask == std::vector<bool>({1, 1, 1, 1, 1, 1, 0, 1, 1, 1}));
    ASSERT_TRUE(PrimitivesEqual(shape3x2x1.m_nodes[0].node.bounds, Sphere(glm::vec3(1.5f, 1.0f, 0.5f), 1.87f), 0.01f));
    ASSERT_TRUE(PrimitivesEqual(shape3x2x1.m_nodes[1].node.bounds, Sphere(glm::vec3(1.0f, 1.0f, 0.5f), 1.5f), 0.01f));
    ASSERT_TRUE(PrimitivesEqual(shape3x2x1.m_nodes[2].node.bounds, Sphere(glm::vec3(2.5f, 1.0f, 0.5f), 1.22f), 0.01f));
    ASSERT_TRUE(PrimitivesEqual(shape3x2x1.m_nodes[3].node.bounds, Sphere(glm::vec3(0.5f, 1.0f, 0.5f), 1.22f), 0.01f));
    ASSERT_TRUE(PrimitivesEqual(shape3x2x1.m_nodes[4].node.bounds, Sphere(glm::vec3(1.5f, 1.0f, 0.5f), 1.22f), 0.01f));
    ASSERT_TRUE(shape3x2x1.m_nodes[5].isLeaf && shape3x2x1.m_nodes[5].leaf.voxel == glm::uvec3(2, 0, 0));
    ASSERT_TRUE(shape3x2x1.m_nodes[7].isLeaf && shape3x2x1.m_nodes[7].leaf.voxel == glm::uvec3(0, 0, 0));
    ASSERT_TRUE(shape3x2x1.m_nodes[8].isLeaf && shape3x2x1.m_nodes[8].leaf.voxel == glm::uvec3(0, 1, 0));
    ASSERT_TRUE(shape3x2x1.m_nodes[9].isLeaf && shape3x2x1.m_nodes[9].leaf.voxel == glm::uvec3(1, 0, 0));
}

TEST_F(VoxelClusterShapeTest, RayCast)
{
    glm::uvec3 voxel;

    ASSERT_FALSE(shape1x1x1.rayCast({}, Ray3D(glm::vec3(-0.1f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f)), voxel));
    ASSERT_TRUE(shape1x1x1.rayCast({}, Ray3D(glm::vec3(0.5f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(0, 0, 0));

    ASSERT_FALSE(shape2x1x1.rayCast({}, Ray3D(glm::vec3(-0.1f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f)), voxel));
    ASSERT_TRUE(shape2x1x1.rayCast({}, Ray3D(glm::vec3(0.5f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(0, 0, 0));
    ASSERT_TRUE(shape2x1x1.rayCast({}, Ray3D(glm::vec3(1.5f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(1, 0, 0));

    ASSERT_FALSE(shape3x2x1.rayCast({}, Ray3D(glm::vec3(-0.1f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f)), voxel));
    ASSERT_TRUE(shape3x2x1.rayCast({}, Ray3D(glm::vec3(0.5f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(0, 0, 0));
    ASSERT_TRUE(shape3x2x1.rayCast({}, Ray3D(glm::vec3(1.5f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(1, 0, 0));
    ASSERT_TRUE(shape3x2x1.rayCast({}, Ray3D(glm::vec3(0.5f, 1.5f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(0, 1, 0));
    ASSERT_FALSE(shape3x2x1.rayCast({}, Ray3D(glm::vec3(1.5f, 1.5f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f)), voxel));

    ASSERT_TRUE(shape3x2x1.rayCast({}, Ray3D(glm::vec3(3.5f, 0.5f, 0.5f), glm::vec3(-3.0f, 0.0f, 0.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(2, 0, 0));
}
