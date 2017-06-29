#include "VoxelObjectPrototype.h"

#include "ColorPalette.h"
#include "VoxReader.h"

#define VERBOSE 0

std::shared_ptr<VoxelObjectVoxelData> VoxelObjectVoxelData::fromFile(VoxReader & voxReader,
                                                                     VoxelWorld & voxelWorld,
                                                                     const std::string & path)
{
    auto models = voxReader.read(path);
    if (!models.empty())
    {
        auto palette = std::make_shared<ColorPalette>(voxelWorld.drawContext(), models[0].palette);

        auto voxelData = std::make_shared<VoxelObjectVoxelData>(voxelWorld,
                                                                palette,
                                                                models[0].size);

        voxelData->addVoxelsRaw(models[0].voxels);
        return voxelData;
    }

    return {};
}

VoxelObjectVoxelData::VoxelObjectVoxelData(const VoxelObjectVoxelData & prototype):
    m_voxelWorld(prototype.m_voxelWorld),
    m_colorIndices(prototype.m_colorIndices),
    m_healthPoints(prototype.m_healthPoints),
    m_renderable(prototype.m_renderable),
    m_shape(std::make_shared<VoxelShape>(*prototype.m_shape)),
    m_hull(prototype.m_hull),
    m_numVoxels(prototype.m_numVoxels),
    m_splitDetector(prototype.m_splitDetector)
{

}

VoxelObjectVoxelData::VoxelObjectVoxelData(VoxelWorld & voxelWorld,
                                           const std::shared_ptr<ColorPalette> & palette,
                                           const glm::uvec3 & size):
    m_voxelWorld(voxelWorld),
    m_colorIndices(size),
    m_healthPoints(size),
    m_renderable(voxelWorld, palette, size),
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
    return m_colorIndices.size();
}

const VoxelRenderable & VoxelObjectVoxelData::renderTree() const
{
    return m_renderable;
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
    return m_colorIndices.contains(voxel) && m_colorIndices.test(voxel);
}

u32 VoxelObjectVoxelData::voxelColorIndex(const glm::uvec3 & voxel) const
{
    return m_colorIndices.getRef(voxel);
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
    m_renderable.setScale(scale);
    m_shape->setScale(scale);
}

void VoxelObjectVoxelData::addVoxelsRaw(std::vector<Voxel> voxels)
{
#if VERBOSE
    std::cout << "VoxelObjectVoxelData::addVoxels(" << this << ") - Voxels: " << m_numVoxels << " + " << voxels.size() << std::endl;
    for (const auto & voxel : voxels)
    {
        std::cout << "  " << voxel.cell << std::endl;
    }
    std::cout << std::endl;
#endif

    /**
     * Erase already set Voxels from list
     */
    {
        size_t vW = 0;
        for (size_t vR = 0; vR < voxels.size(); vR++)
        {
            if (!hasVoxel(voxels[vR].cell))
            {
                voxels[vW] = voxels[vR];
                vW++;
            }
        }
        voxels.resize(vW);
    }

    m_hull.addVoxels(voxels);

    for (auto & voxel : voxels)
    {
        Assert(!m_colorIndices.test(voxel.cell), "Already contains voxel " + ToString(voxel.cell));

        m_colorIndices.set(voxel.cell, voxel.colorIndex);
        m_healthPoints.set(voxel.cell, voxel.healthPoints);
        m_renderable.addVoxel(voxel, m_hull.isHullVoxel(voxel.cell));
    }

    for (auto & voxel : m_hull.newHullVoxels())
    {
        m_shape->updateVoxel(voxel, true);
    }

    for (auto & voxel : m_hull.newObscuredVoxels())
    {
        m_renderable.updateVoxelVisibility(voxel, false);
        m_shape->updateVoxel(voxel, false);
    }

    m_splitDetector.addVoxels(voxels);

    m_numVoxels += voxels.size();
}

void VoxelObjectVoxelData::removeVoxelsRaw(std::vector<glm::uvec3> voxels)
{
#if VERBOSE
    std::cout << "VoxelObjectVoxelData(" << this << ")::removeVoxels() - Voxels: " << m_numVoxels << " - " << voxels.size() << std::endl;
    for (const auto & voxel : voxels)
    {
        std::cout << "  " << voxel << std::endl;
    }
#endif

    /**
     * Erase not set Voxels from list
     */
    {
        size_t vW = 0;
        for (size_t vR = 0; vR < voxels.size(); vR++)
        {
            if (hasVoxel(voxels[vR]))
            {
                voxels[vW] = voxels[vR];
                vW++;
            }
        }
        voxels.resize(vW);
    }

    for (auto & voxel : voxels)
    {
        Assert(m_colorIndices.test(voxel), "Doesn't contain voxel " + ToString(voxel));

        m_colorIndices.set(voxel, VoxelCluster<u32>::EMPTY_VOXEL);
        m_healthPoints.set(voxel, VoxelCluster<float>::EMPTY_VOXEL);

        if (m_hull.isHullVoxel(voxel)) m_shape->updateVoxel(voxel, false);
        m_renderable.removeVoxel(voxel, m_hull.isHullVoxel(voxel));
    }

    m_hull.removeVoxels(voxels);

    for (auto & voxel : m_hull.newHullVoxels())
    {
        m_shape->updateVoxel(voxel, true);
        m_renderable.updateVoxelVisibility(voxel, true);
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