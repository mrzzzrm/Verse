#include "VoxelObjectPrototype.h"

#include "VoxReader.h"

#define VERBOSE 0

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
    m_numVoxels(prototype.m_numVoxels),
    m_splitDetector(prototype.m_splitDetector)
{

}

VoxelObjectVoxelData::VoxelObjectVoxelData(VoxelWorld & voxelWorld, const glm::uvec3 & size):
    m_voxelWorld(voxelWorld),
    m_colors(size),
    m_healthPoints(size),
    m_renderTree(voxelWorld, size),
    m_shape(std::make_shared<VoxelShape>(size)),
    m_hull(size),
    m_splitDetector(size)
{

}

VoxelWorld & VoxelObjectVoxelData::voxelWorld() const
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

float VoxelObjectVoxelData::scale() const
{
    return m_scale;
}

bool VoxelObjectVoxelData::hasVoxel(const glm::ivec3 & voxel) const
{
    return m_colors.contains(voxel) && m_colors.test(voxel);
}

const glm::vec3 & VoxelObjectVoxelData::voxelColor(const glm::uvec3 & voxel) const
{
    return m_colors.getRef(voxel);
}

void VoxelObjectVoxelData::setVoxelColor(const glm::uvec3 & voxel, const glm::vec3 & color)
{
    m_colors.set(voxel, color);
    m_renderTree.invalidateVoxel(voxel);
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
#if VERBOSE
    std::cout << "VoxelObjectVoxelData::addVoxels(" << this << ") - Voxels: " << m_numVoxels << " + " << voxels.size() << std::endl;
    for (const auto & voxel : voxels)
    {
        std::cout << "  " << voxel.cell << std::endl;
    }
    std::cout << std::endl;
#endif

    m_hull.addVoxels(voxels);

    for (auto & voxel : voxels)
    {
        Assert(!m_colors.test(voxel.cell), "Already contains voxel " + ToString(voxel.cell));

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
#if VERBOSE
    std::cout << "VoxelObjectVoxelData(" << this << ")::removeVoxels() - Voxels: " << m_numVoxels << " - " << voxels.size() << std::endl;
    for (const auto & voxel : voxels)
    {
        std::cout << "  " << voxel << std::endl;
    }
#endif

    for (auto & voxel : voxels)
    {
        Assert(m_colors.test(voxel), "Doesn't contain voxel " + ToString(voxel));

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

    Assert(m_numVoxels >= voxels.size(), "");
    m_numVoxels -= voxels.size();

#if VERBOSE
    std::cout << "Remaining Voxels: " << m_numVoxels << ", removed: " << voxels.size() << std::endl;
    std::cout << std::endl;
#endif
}

void VoxelObjectVoxelData::setCrucialVoxel(const boost::optional<glm::uvec3> & crucialVoxel)
{
    m_splitDetector.setCrucialVoxel(crucialVoxel);
}