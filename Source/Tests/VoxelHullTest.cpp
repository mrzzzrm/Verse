#include <Deliberation/Core/Math/PrimitiveCompare.h>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/Math/Sphere.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include "Voxel.h"
#include "VoxelHull.h"

#include <memory>

#include "gtest/gtest.h"

class VoxelHullTest : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
    }

};

TEST_F(VoxelHullTest, SingleVoxel)
{
    std::vector<Voxel> voxels({Voxel({0, 0, 0})});

    VoxelHull hull({1,1,1});

    hull.addVoxels(voxels);
    ASSERT_EQ(hull.newHullVoxels().size(), 1);
    ASSERT_EQ(hull.newHullVoxels()[0], glm::uvec3(0, 0, 0));
    ASSERT_EQ(hull.newObscuredVoxels().size(), 0);

    hull.removeVoxels({glm::uvec3(0, 0, 0)});
    ASSERT_EQ(hull.newHullVoxels().size(), 0);
    ASSERT_EQ(hull.newObscuredVoxels().size(), 0);
}

TEST_F(VoxelHullTest, Cube)
{
    std::vector<Voxel> voxels;

    for (u32 x = 0; x < 3; x++)
    for (u32 y = 0; y < 3; y++)
    for (u32 z = 0; z < 3; z++)
    voxels.emplace_back(Voxel({x, y, z}));

    VoxelHull hull({3, 3, 3});

    hull.addVoxels(voxels);
    ASSERT_EQ(hull.newHullVoxels().size(), 26);
    ASSERT_EQ(hull.newObscuredVoxels().size(), 1);

    hull.removeVoxels({glm::uvec3(1,1,0)});
    ASSERT_EQ(hull.newHullVoxels().size(), 1);
    ASSERT_EQ(hull.newObscuredVoxels().size(), 0);

    hull.removeVoxels({glm::uvec3(1,1,1)});
    ASSERT_EQ(hull.newHullVoxels().size(), 0);
    ASSERT_EQ(hull.newObscuredVoxels().size(), 0);

}
