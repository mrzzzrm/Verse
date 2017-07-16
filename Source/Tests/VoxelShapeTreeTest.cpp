#include <Deliberation/Core/Math/PrimitiveCompare.h>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/Math/Sphere.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include "Voxel.h"
#include "VoxelCluster.h"
#include "VoxelShape.h"

#include <memory>

#include "gtest/gtest.h"

class VoxelShapeTreeTest : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        std::vector<Voxel> voxels1x1x1({Voxel({0,0,0})});
        std::vector<Voxel> voxels2x1x1({Voxel({0,0,0}), Voxel({1,0,0})});
        std::vector<Voxel> voxels3x2x1({Voxel({0,0,0}), Voxel({1,0,0}), Voxel({2,0,0}), Voxel({0,1,0})});

        cluster1x1x1 = VoxelCluster<bool>({1, 1, 1});
        shape1x1x1.reset(glm::uvec3(1, 1, 1));
        for (auto & voxel : voxels1x1x1)
        {
            cluster1x1x1.set(voxel.cell, true);
            shape1x1x1->updateVoxel(voxel.cell, true);
        }
        
        cluster2x1x1 = VoxelCluster<bool>({2, 1, 1});
        shape2x1x1.reset(glm::uvec3(2, 1, 1));
        for (auto & voxel : voxels2x1x1)
        {
            cluster2x1x1.set(voxel.cell, true);
            shape2x1x1->updateVoxel(voxel.cell, true);
        }
        
        cluster3x2x1 = VoxelCluster<bool>({3, 2, 1});
        shape3x2x1.reset(glm::uvec3(3, 2, 1));
        for (auto & voxel : voxels3x2x1)
        {
            cluster3x2x1.set(voxel.cell, true);
            shape3x2x1->updateVoxel(voxel.cell, true);
        }
    }

    VoxelCluster<bool> cluster1x1x1;
    Optional<VoxelShape> shape1x1x1;
    VoxelCluster<bool> cluster2x1x1;
    Optional<VoxelShape> shape2x1x1;
    VoxelCluster<bool> cluster3x2x1;
    Optional<VoxelShape> shape3x2x1;
};

TEST_F(VoxelShapeTreeTest, Basic)
{
//    ASSERT_TRUE(shape1x1x1->size() == glm::uvec3(1, 1, 1));
//    ASSERT_TRUE(shape1x1x1.m_nodeMask == std::vector<bool>({true}));
//    ASSERT_TRUE(shape1x1x1.m_nodes[0].leaf.voxel == glm::uvec3(0, 0, 0));
//
//    ASSERT_TRUE(shape2x1x1.size() == glm::uvec3(2, 1, 1));
//    ASSERT_TRUE(shape2x1x1.m_nodeMask == std::vector<bool>({true, true, true}));
//    ASSERT_TRUE(PrimitivesEqual(shape2x1x1.m_nodes[0].node.bounds, Sphere(glm::vec3(1.0f, 0.5f, 0.5f), 1.22f), 0.01f));
//    ASSERT_TRUE(shape2x1x1.m_nodes[1].leaf.voxel == glm::uvec3(0, 0, 0));
//    ASSERT_TRUE(shape2x1x1.m_nodes[2].leaf.voxel == glm::uvec3(1, 0, 0));
//
//    ASSERT_TRUE(shape3x2x1.size() == glm::uvec3(3, 2, 1));
//    ASSERT_TRUE(shape3x2x1.m_nodeMask == std::vector<bool>({1, 1, 1, 1, 1, 1, 0, 1, 1, 1}));
//    ASSERT_TRUE(PrimitivesEqual(shape3x2x1.m_nodes[0].node.bounds, Sphere(glm::vec3(1.5f, 1.0f, 0.5f), 1.87f), 0.01f));
//    ASSERT_TRUE(PrimitivesEqual(shape3x2x1.m_nodes[1].node.bounds, Sphere(glm::vec3(1.0f, 1.0f, 0.5f), 1.5f), 0.01f));
//    ASSERT_TRUE(PrimitivesEqual(shape3x2x1.m_nodes[2].node.bounds, Sphere(glm::vec3(2.5f, 1.0f, 0.5f), 1.22f), 0.01f));
//    ASSERT_TRUE(PrimitivesEqual(shape3x2x1.m_nodes[3].node.bounds, Sphere(glm::vec3(0.5f, 1.0f, 0.5f), 1.22f), 0.01f));
//    ASSERT_TRUE(PrimitivesEqual(shape3x2x1.m_nodes[4].node.bounds, Sphere(glm::vec3(1.5f, 1.0f, 0.5f), 1.22f), 0.01f));
//    ASSERT_TRUE(shape3x2x1.m_nodes[5].isLeaf && shape3x2x1.m_nodes[5].leaf.voxel == glm::uvec3(2, 0, 0));
//    ASSERT_TRUE(shape3x2x1.m_nodes[7].isLeaf && shape3x2x1.m_nodes[7].leaf.voxel == glm::uvec3(0, 0, 0));
//    ASSERT_TRUE(shape3x2x1.m_nodes[8].isLeaf && shape3x2x1.m_nodes[8].leaf.voxel == glm::uvec3(0, 1, 0));
//    ASSERT_TRUE(shape3x2x1.m_nodes[9].isLeaf && shape3x2x1.m_nodes[9].leaf.voxel == glm::uvec3(1, 0, 0));
}

TEST_F(VoxelShapeTreeTest, RayCast)
{
    glm::uvec3 voxel;

    ASSERT_FALSE(shape1x1x1->lineCast({}, Ray3D(glm::vec3(-0.4f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_TRUE(shape1x1x1->lineCast({}, Ray3D(glm::vec3(0.5f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(0, 0, 0));

    ASSERT_FALSE(shape2x1x1->lineCast({}, Ray3D(glm::vec3(-0.4f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_TRUE(shape2x1x1->lineCast({}, Ray3D(glm::vec3(0.5f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(0, 0, 0));
    ASSERT_TRUE(shape2x1x1->lineCast({}, Ray3D(glm::vec3(1.5f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(1, 0, 0));

    ASSERT_FALSE(shape3x2x1->lineCast({}, Ray3D(glm::vec3(-0.4f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_TRUE(shape3x2x1->lineCast({}, Ray3D(glm::vec3(0.5f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(0, 0, 0));
    ASSERT_TRUE(shape3x2x1->lineCast({}, Ray3D(glm::vec3(1.5f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(1, 0, 0));
    ASSERT_TRUE(shape3x2x1->lineCast({}, Ray3D(glm::vec3(0.5f, 1.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(0, 1, 0));
    ASSERT_FALSE(shape3x2x1->lineCast({}, Ray3D(glm::vec3(1.5f, 1.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));

    ASSERT_TRUE(shape3x2x1->lineCast({}, Ray3D(glm::vec3(3.5f, 0.5f, 0.5f), glm::vec3(-30.0f, 0.0f, 0.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(2, 0, 0));
}

TEST_F(VoxelShapeTreeTest, RayCastWithScale)
{
    glm::uvec3 voxel;

    Transform3D transform;
    transform.setScale(2.0f);

    EXPECT_FALSE(shape1x1x1->lineCast(transform, Ray3D(glm::vec3(-0.6f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    EXPECT_FALSE(shape1x1x1->lineCast(transform, Ray3D(glm::vec3(2.6f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    EXPECT_TRUE(shape1x1x1->lineCast(transform, Ray3D(glm::vec3(1.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    EXPECT_EQ(voxel, glm::uvec3(0, 0, 0));

    EXPECT_FALSE(shape2x1x1->lineCast(transform, Ray3D(glm::vec3(-0.6f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    EXPECT_TRUE(shape2x1x1->lineCast(transform, Ray3D(glm::vec3(1.0f, 1.0f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    EXPECT_EQ(voxel, glm::uvec3(0, 0, 0));
    EXPECT_TRUE(shape2x1x1->lineCast(transform, Ray3D(glm::vec3(1.5f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    EXPECT_EQ(voxel, glm::uvec3(0, 0, 0));
    EXPECT_TRUE(shape2x1x1->lineCast(transform, Ray3D(glm::vec3(3.0f, 0.5f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    EXPECT_EQ(voxel, glm::uvec3(1, 0, 0));

    ASSERT_FALSE(shape3x2x1->lineCast(transform, Ray3D(glm::vec3(-0.8f, 1.0f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_TRUE(shape3x2x1->lineCast(transform, Ray3D(glm::vec3(1.0f, 1.0f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(0, 0, 0));
    ASSERT_TRUE(shape3x2x1->lineCast(transform, Ray3D(glm::vec3(3.0f, 1.0f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(1, 0, 0));
    ASSERT_TRUE(shape3x2x1->lineCast(transform, Ray3D(glm::vec3(1.0f, 3.0f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(0, 1, 0));
    ASSERT_FALSE(shape3x2x1->lineCast(transform, Ray3D(glm::vec3(3.0f, 3.0f, -0.1f), glm::vec3(0.0f, 0.0f, 10.0f)), voxel));

    ASSERT_TRUE(shape3x2x1->lineCast(transform, Ray3D(glm::vec3(7.0f, 1.0f, 0.5f), glm::vec3(-30.0f, 0.0f, 0.0f)), voxel));
    ASSERT_EQ(voxel, glm::uvec3(2, 0, 0));
}