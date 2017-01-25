#include "VoxelRenderChunkTree.h"

#include <sstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/ScopeProfiler.h>

VoxelRenderChunkTree::VoxelRenderChunkTree(const VoxelWorld & voxelWorld, const glm::uvec3 & size):
    m_voxelWorld(voxelWorld),
    m_size(size)
{
    ScopeProfiler scopeProfiler("VoxelRenderChunkTree::VoxelRenderChunkTree()");

    Assert(m_size.x >= 0 && m_size.y >= 0 && m_size.z >= 0, "");

    size_t numChunks = 0;

    glm::uvec3 maxChunkSize(6); // Must be > 2

    std::function<void(u32, const glm::ivec3 &, const glm::ivec3 &, const glm::ivec3 &, const glm::ivec3)> buildTree = [&] (
        u32 index,
        const glm::ivec3 & llf,
        const glm::ivec3 & urb,
        const glm::ivec3 & llfRender,
        const glm::ivec3 & urbRender
    )
    {
        if (index >= m_nodes.size()) m_nodes.resize(index + 1);

        auto & node = m_nodes[index];

        node.llf = llf;
        node.urb = urb;
        node.llfRender = llfRender;
        node.urbRender = urbRender;

        auto size = urb - llf + 1;
        auto renderSize = urbRender - llfRender + 1;

        std::cout << index << node.llf << node.urb << node.llfRender<<node.urbRender << std::endl;

        if (renderSize.x <= maxChunkSize.x &&
            renderSize.y <= maxChunkSize.y &&
            renderSize.z <= maxChunkSize.z)
        {
            node.chunk = numChunks;
            numChunks++;
            node.leaf = true;
        }
        else
        {
            auto longestAxis = 0;

            if (renderSize.x >= renderSize.y && renderSize.x >= renderSize.z) longestAxis = 0;
            else if (renderSize.y >= renderSize.x && renderSize.y >= renderSize.z) longestAxis = 1;
            else longestAxis = 2;

            auto separationIndex = (llfRender[longestAxis] + urbRender[longestAxis]) / 2;
            std::cout << "  Seperating at " << separationIndex << std::endl;

            auto urbLeft = urb;
            urbLeft[longestAxis] = std::min(separationIndex + 1, urb[longestAxis]);

            auto urbRenderLeft = urbRender;
            urbRenderLeft[longestAxis] = separationIndex;

            auto llfRight = llf;
            llfRight[longestAxis] = std::max(separationIndex - 1, llf[longestAxis]);

            auto llfRenderRight = llfRender;
            llfRenderRight[longestAxis] = std::min(separationIndex + 1, urbRender[longestAxis]);

            buildTree(index * 2 + 1, llf, urbLeft, llfRender, urbRenderLeft);
            buildTree(index * 2 + 2, llfRight, urb, llfRenderRight, urbRender);
        }
    };

    auto llf = glm::ivec3(0);
    auto urb = glm::ivec3(m_size) - 1;
    buildTree(0, llf, urb, llf, urb);

    m_chunks.resize(numChunks);
    m_nodeMask.resize(m_nodes.size());
}


void VoxelRenderChunkTree::addVoxel(const Voxel & voxel, bool visible)
{
    addVoxelToNode(0, voxel, visible);
}

void VoxelRenderChunkTree::removeVoxel(const glm::uvec3 & voxel, bool visible)
{
    removeVoxelFromNode(0, voxel, visible);
}

void VoxelRenderChunkTree::updateVoxelVisibility(const glm::uvec3 & voxel, bool visible)
{
    updateVoxelVisibilityInNode(0, voxel, visible);
}

void VoxelRenderChunkTree::schedule(const Pose3D & pose) const
{
    //ScopeProfiler scopeProfiler("VoxelRenderChunkTree::schedule()");

    for (auto & chunk : m_chunks)
    {
        if (!chunk.chunk) continue;

        Pose3D chunkPose(pose);
        chunkPose.setPosition(pose.position() + pose.orientation() * chunk.position);

        chunk.chunk->schedule(chunkPose);
    }
}

std::string VoxelRenderChunkTree::toString() const
{
    std::stringstream stream;

    std::function<void(u32, u32)> nodeToString = [&] (u32 index, u32 indent)
    {
        for (u32 i = 0; i < indent; i++) stream << "  ";

        auto & node = m_nodes[index];

        stream << "Node " << node.llf << " -> " << node.urb << std::endl;

        auto l = index * 2 + 1;
        auto r = index * 2 + 2;

        if (!node.leaf) nodeToString(l, indent + 1);
        if (!node.leaf) nodeToString(r, indent + 1);
    };

    nodeToString(0, 0);

    return stream.str();
}

void VoxelRenderChunkTree::addVoxelToNode(u32 index, const Voxel & voxel, bool visible)
{
    if (!isVoxelInNode(index, voxel.cell)) return;

    auto & node = m_nodes[index];

    visible = visible && isVoxelRenderedByNode(index, voxel.cell);

    if (visible) node.numVisibleVoxels++;

    if (node.leaf)
    {
        auto & chunk = m_chunks[node.chunk];

        if (chunk.chunk)
        {
            if (chunk.chunk.use_count() > 1)
            {
                chunk.chunk = chunk.chunk->clone();
            }
        }
        else
        {
            chunk.position = node.llf;
            chunk.chunk = std::make_shared<VoxelRenderChunk>(m_voxelWorld,
                                                             glm::uvec3(node.urb - node.llf + glm::ivec3(1)),
                                                             node.llfRender - node.llf, node.urbRender - node.llf,
                                                             Optional<glm::vec3>(m_colorGenerator.generate()));
        }

        Voxel chunkLocalVoxel = voxel;
        chunkLocalVoxel.cell -= node.llf;

        chunk.chunk->addVoxel(chunkLocalVoxel, visible);
    }
    else
    {
        addVoxelToNode(index * 2 + 1, voxel, visible);
        addVoxelToNode(index * 2 + 2, voxel, visible);
    }
}

void VoxelRenderChunkTree::removeVoxelFromNode(u32 index, const glm::uvec3 & voxel, bool visible)
{
    if (!isVoxelInNode(index, voxel)) return;

    auto & node = m_nodes[index];

    visible = visible && isVoxelRenderedByNode(index, voxel);

    if (visible)
    {
        Assert(node.numVisibleVoxels > 0, "");
        node.numVisibleVoxels--;
    }

    if (node.leaf)
    {
        Assert(!!m_chunks[node.chunk].chunk, "");
        m_chunks[node.chunk].chunk->removeVoxel(voxel - glm::uvec3(node.llf),
                                                visible);
    }
    else
    {
        removeVoxelFromNode(index * 2 + 1, voxel, visible);
        removeVoxelFromNode(index * 2 + 2, voxel, visible);
    }
}

void VoxelRenderChunkTree::updateVoxelVisibilityInNode(size_t index, const glm::uvec3 & voxel, bool visible)
{
    if (!isVoxelRenderedByNode(index, voxel)) return;

    auto & node = m_nodes[index];

    if (visible)
    {
        node.numVisibleVoxels++;
    }
    else
    {
        Assert(node.numVisibleVoxels > 0, "");
        node.numVisibleVoxels--;
    }

    if (node.leaf)
    {
        m_chunks[node.chunk].chunk->updateVoxelVisibility(voxel - glm::uvec3(node.llf), visible);
    }
    else
    {
        updateVoxelVisibilityInNode(index * 2 + 1, voxel, visible);
        updateVoxelVisibilityInNode(index * 2 + 2, voxel, visible);
    }
}

bool VoxelRenderChunkTree::isVoxelInNode(size_t index, const glm::uvec3 & voxel)
{
    auto & node = m_nodes[index];

    auto c = glm::ivec3(voxel);

    return c.x >= node.llf.x && c.x <= node.urb.x &&
        c.y >= node.llf.y && c.y <= node.urb.y &&
        c.z >= node.llf.z && c.z <= node.urb.z;
}

bool VoxelRenderChunkTree::isVoxelRenderedByNode(size_t index, const glm::uvec3 & voxel)
{
    auto & node = m_nodes[index];

    auto c = glm::ivec3(voxel);

    return c.x >= node.llfRender.x && c.x <= node.urbRender.x &&
        c.y >= node.llfRender.y && c.y <= node.urbRender.y &&
        c.z >= node.llfRender.z && c.z <= node.urbRender.z;
}