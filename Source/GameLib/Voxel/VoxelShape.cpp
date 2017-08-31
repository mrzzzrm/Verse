#include "VoxelShape.h"

#include <functional>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/PrimitiveIntersection.h>
#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/StreamUtils.h>

#include "CollisionShapeTypes.h"

VoxelShape::VoxelShape(const glm::uvec3 & size)
    : CollisionShape((int)::CollisionShapeType::VoxelCluster)
    , m_size(size)
    , m_tree(m_size, glm::uvec3(20))
{
}

AABB VoxelShape::bounds(const Transform3D & transform) const
{
    const auto center = transform.position();
    const auto radius =
        glm::vec3(glm::length(glm::vec3(m_size))) * transform.scale();
    return AABB(center - glm::vec3(radius), center + glm::vec3(radius));
}

glm::mat3 VoxelShape::localInertia() const
{
    if (m_massPropertiesDirty) updateMassProperties();

    return m_localInertia;
}

float VoxelShape::mass() const { return m_numVoxels; }

glm::vec3 VoxelShape::centerOfMass() const
{
    if (m_massPropertiesDirty) updateMassProperties();

    return m_centerOfMass;
}

void VoxelShape::updateVoxel(const glm::uvec3 & voxel, bool set)
{
    if (set)
    {
        m_numVoxels++;
        m_voxelPositionAccumulator += voxel;

        auto v = voxel;

        /**
         * HACK: Offset by one to avoid zeros on diagonal
         */
        m_absIXX += v.y * v.y + v.z * v.z + 1;
        m_absIYY += v.x * v.x + v.z * v.z + 1;
        m_absIZZ += v.x * v.x + v.y * v.y + 1;

        m_absIXY += v.x * v.y;
        m_absIXZ += v.x * v.z;
        m_absIYZ += v.y * v.z;
    }
    else
    {
        AssertM(m_numVoxels > 0, "No voxels to remove");
        m_numVoxels--;
        m_voxelPositionAccumulator -= voxel;

        auto v = voxel;

        /**
         * HACK: Offset by one to avoid zeros on diagonal
         */
        m_absIXX -= v.y * v.y + v.z * v.z + 1;
        m_absIYY -= v.x * v.x + v.z * v.z + 1;
        m_absIZZ -= v.x * v.x + v.y * v.y + 1;

        m_absIXY -= v.x * v.y;
        m_absIXZ -= v.x * v.z;
        m_absIYZ -= v.y * v.z;
    }

    m_massPropertiesDirty = true;

    m_tree.updateVoxel(0, voxel, set);
}

bool VoxelShape::lineCast(
    const Transform3D & transform, const Ray3D & ray, glm::uvec3 & voxel) const
{
    std::vector<glm::uvec3> voxels;

    auto localOrigin = transform.pointWorldToLocal(ray.origin());
    auto localDirection = transform.directionWorldToLocal(ray.direction());

    m_tree.lineCast(0, Ray3D(localOrigin, localDirection), voxels);

    if (voxels.empty()) return false;

    auto minimumSquaredDistance = std::numeric_limits<float>::max();
    auto minimumDistanceVoxel = size_t(0);

    for (size_t v = 0; v < voxels.size(); v++)
    {
        auto position = glm::vec3(voxels[v]) + glm::vec3(0.5f);
        auto delta = position - localOrigin;
        auto squaredDistance = glm::dot(delta, delta);

        if (squaredDistance < minimumSquaredDistance)
        {
            minimumSquaredDistance = squaredDistance;
            minimumDistanceVoxel = v;
        }
    }

    voxel = voxels[minimumDistanceVoxel];

    return true;
}

std::string VoxelShape::toString() const
{
    std::stringstream stream;
    m_tree.toString(stream, 0, 0);
    return stream.str();
}

template<typename T>
VoxelShape::Subtree<T>::Subtree(
    const glm::uvec3 & size, const glm::uvec3 & maxChunkSize)
{
    Assert(size.x >= 0 && size.y >= 0 && size.z >= 0);

    size_t numLeafs = 0;

    std::function<void(size_t, const glm::ivec3 &, const glm::ivec3 &)>
        buildTree = [&](size_t             index,
                        const glm::ivec3 & llf,
                        const glm::ivec3 & urb) {
            if (index >= nodes.size()) nodes.resize(index + 1);

            auto & node = nodes[index];

            node.llf = llf;
            node.urb = urb;

            auto nodeSize = urb - llf + 1;

            auto position = glm::vec3(llf) + glm::vec3(nodeSize) / 2.0f;
            auto radius = glm::length(glm::vec3(nodeSize) / 2.0f);

            node.bounds = Sphere(position, radius);

            if (nodeSize.x <= maxChunkSize.x && nodeSize.y <= maxChunkSize.y &&
                nodeSize.z <= maxChunkSize.z)
            {
                node.leaf = numLeafs;
                numLeafs++;
            }
            else
            {
                auto longestAxis = 0;

                if (nodeSize.x >= nodeSize.y && nodeSize.x >= nodeSize.z)
                    longestAxis = 0;
                else if (nodeSize.y >= nodeSize.x && nodeSize.y >= nodeSize.z)
                    longestAxis = 1;
                else
                    longestAxis = 2;

                auto separationIndex =
                    (urb[longestAxis] + llf[longestAxis]) / 2;

                auto urbLeft = urb;
                urbLeft[longestAxis] = separationIndex;

                auto llfRight = llf;
                llfRight[longestAxis] = separationIndex + 1;

                buildTree(index * 2 + 1, llf, urbLeft);
                buildTree(index * 2 + 2, llfRight, urb);
            }
        };

    auto llf = glm::ivec3(0);
    auto urb = glm::ivec3(size) - 1;

    buildTree(0, llf, urb);

    leaves.resize(numLeafs);
}

template<typename T>
void VoxelShape::Subtree<T>::updateVoxel(
    size_t index, const glm::uvec3 & voxel, bool set)
{
    auto & node = nodes[index];

    if (set)
        node.numVoxels++;
    else
        node.numVoxels--;

    if (node.leaf != NO_LEAF)
    {
        updateVoxelLeaf(index, voxel, set);
    }
    else
    {
        auto & c = voxel;
        auto   leftIndex = index * 2 + 1;
        auto & left = nodes[leftIndex];

        if (c.x >= left.llf.x && c.x <= left.urb.x && c.y >= left.llf.y &&
            c.y <= left.urb.y && c.z >= left.llf.z && c.z <= left.urb.z)
        {
            updateVoxel(index * 2 + 1, voxel, set);
        }
        else
        {
            updateVoxel(index * 2 + 2, voxel, set);
        }
    }
}

template<>
void VoxelShape::Subtree<VoxelShape::ChunkLeaf>::updateVoxelLeaf(
    size_t index, const glm::uvec3 & voxel, bool set)
{
    auto & node = nodes[index];
    auto & leaf = leaves[node.leaf];

    if (leaf)
    {
        if (leaf.use_count() > 1)
        {
            leaf = leaf->clone();
        }
    }
    else
    {
        auto maxChunkSize = glm::uvec3(1);
        leaf = std::make_shared<Subtree<VoxelLeaf>>(
            node.urb - node.llf + glm::uvec3(1), maxChunkSize);
    }

    leaf->updateVoxel(0, voxel - node.llf, set);
}

template<>
void VoxelShape::Subtree<VoxelShape::VoxelLeaf>::updateVoxelLeaf(
    size_t index, const glm::uvec3 & voxel, bool set)
{
    Assert(leaves[nodes[index].leaf] != set);
    leaves[nodes[index].leaf] = set;
}

template<typename T>
void VoxelShape::Subtree<T>::lineCast(
    size_t index, const Ray3D & ray, std::vector<glm::uvec3> & voxels) const
{
    auto & node = nodes[index];

    if (node.numVoxels == 0) return;

    if (node.leaf != NO_LEAF)
    {
        lineCastLeaf(index, ray, voxels);
    }
    else
    {
        auto position = node.bounds.position();
        auto radius = node.bounds.radius();

        if (!LineSphereIntersection(
                ray.origin(), ray.direction(), position, radius))
        {
            return;
        }

        auto leftIndex = index * 2 + 1;
        auto rightIndex = leftIndex + 1;

        if (nodes[leftIndex].numVoxels > 0)
        {
            lineCast(leftIndex, ray, voxels);
        }
        if (nodes[rightIndex].numVoxels > 0)
        {
            lineCast(rightIndex, ray, voxels);
        }
    }
}

template<typename T>
void VoxelShape::Subtree<T>::toString(
    std::stringstream & stream, size_t index, size_t indentation) const
{
    auto & node = nodes[index];

    for (auto i = 0; i < indentation; i++)
        stream << "  ";

    stream << index << ": " << node.llf << "->" << node.urb << "; "
           << node.numVoxels << " " << std::endl;

    if (node.leaf != NO_LEAF)
    {
        leafToString(stream, index, indentation + 1);
    }
    else
    {
        toString(stream, index * 2 + 1, indentation + 1);
        toString(stream, index * 2 + 2, indentation + 1);
    }
}

template<>
void VoxelShape::Subtree<VoxelShape::ChunkLeaf>::leafToString(
    std::stringstream & stream, size_t index, size_t indentation) const
{
    auto & node = nodes[index];
    auto & leaf = leaves[node.leaf];

    if (leaf)
        leaves[node.leaf]->toString(stream, 0, indentation);
    else
        stream << "<Empty>" << std::endl;
}

template<>
void VoxelShape::Subtree<VoxelShape::VoxelLeaf>::leafToString(
    std::stringstream & stream, size_t index, size_t indentation) const
{
    auto & node = nodes[index];
    for (auto i = 0; i < indentation; i++)
        stream << "  ";

    stream << "Voxel: " << leaves[node.leaf] << std::endl;
}

template<>
void VoxelShape::Subtree<VoxelShape::ChunkLeaf>::lineCastLeaf(
    size_t index, const Ray3D & ray, std::vector<glm::uvec3> & voxels) const
{
    auto & node = nodes[index];

    Ray3D chunkLocalRay(ray.origin() - glm::vec3(node.llf), ray.direction());

    auto prevNumVoxels = voxels.size();

    leaves[node.leaf]->lineCast(0, chunkLocalRay, voxels);

    for (size_t v = prevNumVoxels; v < voxels.size(); v++)
    {
        voxels[v] += node.llf;
    }
}

template<>
void VoxelShape::Subtree<VoxelShape::VoxelLeaf>::lineCastLeaf(
    size_t index, const Ray3D & ray, std::vector<glm::uvec3> & voxels) const
{
    auto & node = nodes[index];
    auto   leaf = leaves[node.leaf];

    if (!leaf) return;

    if (LineSphereIntersection(
            ray.origin(),
            ray.direction(),
            glm::vec3(node.llf) + glm::vec3(0.5f),
            0.8f))
    {
        voxels.emplace_back(node.llf);
    }
}

template<typename T>
std::shared_ptr<VoxelShape::Subtree<T>> VoxelShape::Subtree<T>::clone() const
{
    return std::make_shared<Subtree<T>>(*this);
}

void VoxelShape::updateMassProperties() const
{
    if (m_numVoxels == 0)
    {
        m_centerOfMass = glm::vec3(0.0f);
        m_localInertia = glm::mat3(1.0f);
        return;
    }

    const auto intCenterOfMass = m_voxelPositionAccumulator / m_numVoxels;
    const auto centerOfMassRemainder =
        glm::vec3(m_voxelPositionAccumulator - intCenterOfMass * m_numVoxels);

    m_centerOfMass =
        glm::vec3(intCenterOfMass) + centerOfMassRemainder / (float)m_numVoxels;

    const auto iXX = m_absIXX -
                     2 * m_centerOfMass.y * m_voxelPositionAccumulator.y -
                     2 * m_centerOfMass.z * m_voxelPositionAccumulator.z +
                     m_numVoxels * (m_centerOfMass.y * m_centerOfMass.y +
                                    m_centerOfMass.z * m_centerOfMass.z);

    const auto iYY = m_absIYY -
                     2 * m_centerOfMass.x * m_voxelPositionAccumulator.x -
                     2 * m_centerOfMass.z * m_voxelPositionAccumulator.z +
                     m_numVoxels * (m_centerOfMass.x * m_centerOfMass.x +
                                    m_centerOfMass.z * m_centerOfMass.z);

    const auto iZZ = m_absIZZ -
                     2 * m_centerOfMass.x * m_voxelPositionAccumulator.x -
                     2 * m_centerOfMass.y * m_voxelPositionAccumulator.y +
                     m_numVoxels * (m_centerOfMass.x * m_centerOfMass.x +
                                    m_centerOfMass.y * m_centerOfMass.y);

    const auto iXY = m_absIXY -
                     m_centerOfMass.y * m_voxelPositionAccumulator.x -
                     m_centerOfMass.x * m_voxelPositionAccumulator.y +
                     m_numVoxels * m_centerOfMass.x * m_centerOfMass.y;

    const auto iXZ = m_absIXZ -
                     m_centerOfMass.z * m_voxelPositionAccumulator.x -
                     m_centerOfMass.x * m_voxelPositionAccumulator.z +
                     m_numVoxels * m_centerOfMass.x * m_centerOfMass.z;

    const auto iYZ = m_absIYZ -
                     m_centerOfMass.z * m_voxelPositionAccumulator.y -
                     m_centerOfMass.y * m_voxelPositionAccumulator.z +
                     m_numVoxels * m_centerOfMass.y * m_centerOfMass.z;

    m_localInertia[0][0] = iXX;
    m_localInertia[1][1] = iYY;
    m_localInertia[2][2] = iZZ;

    m_localInertia[1][0] = -iXY;
    m_localInertia[0][1] = -iXY;

    m_localInertia[2][0] = -iXZ;
    m_localInertia[0][2] = -iXZ;

    m_localInertia[2][1] = -iYZ;
    m_localInertia[1][2] = -iYZ;

    m_massPropertiesDirty = false;
}