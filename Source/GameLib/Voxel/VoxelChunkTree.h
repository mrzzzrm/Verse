//#pragma once
//
//#include <vector>
//
//#include <glm/glm.hpp>
//
//#include "Voxel.h"
//
//template<typename Chunk>
//class VoxelChunkTree {
//public:
//    VoxelChunkTree(const glm::uvec3 & size);
//
//    void addVoxels(const std::vector<Voxel> & voxels);
//    void removeVoxel(const std::vector<glm::uvec3> & voxels);
//
//protected:
//    struct Node
//    {
//        glm::ivec3  llf;
//        glm::ivec3  urb;
//        bool        empty = true;
//        bool        hidden = true;
//        size_t      chunk = std::numeric_limits<size_t>::max();
//        glm::vec3   position;
//        float       radius = 0.0f;
//        bool        leaf = false;
//    };
//
//private:
//    void addVoxelToNode(u32 index, Voxel & voxel);
//    void removeVoxelFromNode(u32 index, const glm::uvec3 & voxel);
//
//protected:
//    glm::uvec3          m_size;
//    std::vector<bool>   m_nodeMask;
//    std::vector<Node>   m_nodes;
//    std::vector<std::shared_ptr<Chunk>>
//                        m_chunks;
//};