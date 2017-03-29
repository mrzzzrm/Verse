#include "VoxelObjectPrototype.h"

#include "VoxReader.h"

std::shared_ptr<VoxelObjectVoxelData> VoxelObjectVoxelData::fromFile(VoxReader & voxReader,
                                                                     VoxelWorld & voxelWorld,
                                                                     const std::string & path)
{
    auto models = voxReader.read(path);
    if (!models.empty())
    {
        auto voxelData = std::make_shared<VoxelObjectVoxelData>(voxelWorld, models[0].size);
        voxelData->addVoxels(models[0].voxels);
        return voxelData;
    }

    return {};
}

VoxelObjectVoxelData::VoxelObjectVoxelData(const VoxelObjectVoxelData & prototype):
    m_voxelWorld(prototype.m_voxelWorld),
    m_colors(prototype.m_colors),
    m_healthPoints(prototype.m_healthPoints),
    m_renderTree(prototype.m_renderTree),
    m_shape(std::make_shared<VoxelShape>(*prototype.m_shape)),
    m_hull(prototype.m_hull),
    m_splitDetector(prototype.m_splitDetector),
    m_numVoxels(prototype.m_numVoxels)
{

}

VoxelObjectVoxelData::VoxelObjectVoxelData(const VoxelWorld & voxelWorld, const glm::uvec3 & size):
    m_voxelWorld(voxelWorld),
    m_colors(size),
    m_healthPoints(size),
    m_renderTree(voxelWorld, size),
    m_shape(std::make_shared<VoxelShape>(size)),
    m_hull(size),
    m_splitDetector(size)
{

}

const VoxelWorld & VoxelObjectVoxelData::voxelWorld() const
{
    return m_voxelWorld;
}

const glm::uvec3 & VoxelObjectVoxelData::size() const
{
    return m_colors.size();
}

const VoxelRenderChunkTree & VoxelObjectVoxelData::renderTree() const
{
    return m_renderTree;
}

const std::shared_ptr<VoxelShape>  & VoxelObjectVoxelData::shape() const
{
    return m_shape;
}

const VoxelHull & VoxelObjectVoxelData::hull() const
{
    return m_hull;
}

const VoxelClusterSplitDetector & VoxelObjectVoxelData::splitDetector() const
{
    return m_splitDetector;
}

float VoxelObjectVoxelData::scale() const
{
    return m_scale;
}

void VoxelObjectVoxelData::setCrucialVoxel(const glm::uvec3 & voxel)
{
    m_splitDetector.setCrucialVoxel(voxel);
}

bool VoxelObjectVoxelData::hasVoxel(const glm::ivec3 & voxel) const
{
    return m_colors.contains(voxel) && m_colors.test(voxel);
}

const glm::vec3 & VoxelObjectVoxelData::voxelColor(const glm::uvec3 & voxel) const
{
    return m_colors.getRef(voxel);
}

float VoxelObjectVoxelData::voxelHealthPoints(const glm::uvec3 & voxel) const
{
    return m_healthPoints.getRef(voxel);
}

void VoxelObjectVoxelData::setVoxelHealthPoints(const glm::uvec3 & voxel, float healthPoints)
{
    m_healthPoints.set(voxel, healthPoints);
}

void VoxelObjectVoxelData::setScale(float scale)
{
    m_scale = scale;
    m_renderTree.setScale(scale);
    m_shape->setScale(scale);
}

void VoxelObjectVoxelData::addVoxels(std::vector<Voxel> voxels)
{
    m_hull.addVoxels(voxels);

    for (auto & voxel : voxels)
    {
        m_colors.set(voxel.cell, voxel.color);
        m_healthPoints.set(voxel.cell, voxel.healthPoints);
        m_renderTree.addVoxel(voxel, m_hull.isHullVoxel(voxel.cell));
    }

    for (auto & voxel : m_hull.newHullVoxels())
    {
        m_shape->updateVoxel(voxel, true);
    }

    for (auto & voxel : m_hull.newObscuredVoxels())
    {
        m_renderTree.updateVoxelVisibility(voxel, false);
        m_shape->updateVoxel(voxel, false);
    }

    m_splitDetector.addVoxels(voxels);

    m_numVoxels += voxels.size();
}

void VoxelObjectVoxelData::removeVoxels(const std::vector<glm::uvec3> & voxels)
{
    for (auto & voxel : voxels)
    {
        m_colors.set(voxel, VoxelCluster<glm::vec3>::EMPTY_VOXEL);
        m_healthPoints.set(voxel, VoxelCluster<float>::EMPTY_VOXEL);

        if (m_hull.isHullVoxel(voxel)) m_shape->updateVoxel(voxel, false);
        m_renderTree.removeVoxel(voxel, m_hull.isHullVoxel(voxel));
    }

    m_hull.removeVoxels(voxels);

    for (auto & voxel : m_hull.newHullVoxels())
    {
        m_shape->updateVoxel(voxel, true);
        m_renderTree.updateVoxelVisibility(voxel, true);
    }

    m_splitDetector.removeVoxels(voxels);

    m_numVoxels -= voxels.size();
}
