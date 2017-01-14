//#include "VoxelChunkTree.h"
//
//VoxelChunkTree::VoxelChunkTree(const glm::uvec3 & size):
//    m_size(size)
//{
//    Assert(m_size.x >= 0 && m_size.y >= 0 && m_size.z >= 0, "");
//
//    size_t numChunks = 0;
//    size_t numNodes = 0;
//
//    glm::uvec3 maxChunkSize(5, 5, 5);
//
//    std::function<void(const glm::uvec3 &, const glm::uvec3 &)> buildTree = [&] (
//        u32 index,
//        const glm::uvec3 & llf,
//        const glm::uvec3 & size)
//    {
//        if (index >= m_nodes.size()) m_nodes.resize(index + 1);
//
//        auto & node = m_nodes[index];
//
//        node.llf = llf;
//        node.urb = llf + m_size - glm::ivec3(1);
//
//        if (size.x <= maxChunkSize.x &&
//            size.y <= maxChunkSize.y &&
//            size.z <= maxChunkSize.z)
//        {
//            node.chunk = numChunks;
//            numChunks++;
//            node.leaf = true;
//        }
//        else
//        {
//            u8 longestAxis = 0;
//
//            if (size.x >= size.y && size.x >= size.z) longestAxis = 0;
//            else if (size.y >= size.x && size.y >= size.z) longestAxis = 1;
//            else longestAxis = 2;
//
//            auto leftSize = size;
//            size[longestAxis] = std::ceil(size[longestAxis] / 2.0f);
//
//            auto rightSize = size;
//            size[longestAxis] = std::floor(size[longestAxis] / 2.0f);
//
//            buildTree(index * 2 + 1, llf, leftSize);
//
//            glm::uvec3 llfOffset;
//            llfOffset[longestAxis] = leftSize[longestAxis];
//
//            buildTree(index * 2 + 1, llf + llfOffset, rightSize);
//        }
//    };
//
//    buildTree(0, {0, 0, 0}, m_size - glm::ivec3(1, 1, 1));
//
//    m_chunks.resize(numChunks);
//    m_nodeMask.resize(m_nodes.size());
//}
//
//void VoxelChunkTree::addVoxels(const std::vector<Voxel> & voxels)
//{
//    for (auto & voxel : voxels)
//    {
//        addVoxelToNode(0, voxel);
//    }
//}
//
//void VoxelChunkTree::removeVoxel(const std::vector<glm::uvec3> & voxels)
//{
//    for (auto & voxel : voxels)
//    {
//        removeVoxelFromNode(0, voxel);
//    }
//}
//
//void VoxelChunkTree::addVoxelToNode(u32 index, Voxel & voxel)
//{
//    auto & node = m_nodes[index];
//
//    auto & c = voxel.cell;
//
//    if (c.x < node.llf.x || c.x > node.urb.x &&
//        c.y < node.llf.y || c.y > node.urb.y &&
//        c.z < node.llf.z || c.z > node.urb.z)
//    {
//        return;
//    }
//
//
//    if (voxel.visible)
//    {
//        node.hidden = false;
//    }
//
//    node.empty = false;
//
//    if (node.leaf)
//    {
//        auto & chunkPtr = m_chunks[node.chunk];
//
//        if (chunkPtr)
//        {
//            if (chunkPtr.use_count() > 1)
//            {
//                chunkPtr = chunkPtr->clone();
//            }
//        }
//        else
//        {
//            chunkPtr = std::make_shared<T>();
//        }
//
//        chunkPtr->addVoxel(voxel);
//    }
//    else
//    {
//        addVoxelToNode(index * 2 + 1, voxel);
//        addVoxelToNode(index * 2 + 2, voxel);
//    }
//}
//
//void VoxelChunkTree::removeVoxelFromNode(u32 index, Voxel & voxel)
//{
//    auto & node = m_nodes[index];
//
//    auto & c = voxel.cell;
//
//    if (c.x < node.llf.x || c.x > node.urb.x &&
//        c.y < node.llf.y || c.y > node.urb.y &&
//        c.z < node.llf.z || c.z > node.urb.z)
//    {
//        return;
//    }
//
//    if (node.leaf)
//    {
//        Assert(m_chunks[node.chunk].get(), "");
//        m_chunks[node.chunk]->removeVoxel(voxel);
//    }
//    else
//    {
//        removeVoxelFromNode(index * 2 + 1, voxel);
//        removeVoxelFromNode(index * 2 + 2, voxel);
//    }
//}