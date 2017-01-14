//#pragma once
//
//#include <glm/glm.hpp>
//
//#include "GameLib.h"
//#include "Voxel.h"
//#include "VoxelClusterChunk.h"
//#include "VoxelRenderChunk.h"
//#include "VoxelShapeChunk.h"
//
//class VoxelObjectVoxelData
//{
//public:
//    VoxelObjectVoxelData(const glm::uvec3 & size);
//    virtual ~VoxelObjectVoxelData() = default;
//
//    void addVoxels(std::vector<Voxel> voxels);
//    void removeVoxels(const std::vector<glm::uvec3> & voxels);
//
//    const glm::uvec3 & size() const;
//
//private:
//    enum class VoxelVisibilityState
//    {
//        NoVoxel,
//        NotSet,
//        Hidden,
//        Visible
//    };
//
//private:
//    glm::uvec3                          m_size;
//    VoxelCluster<VoxelVisibilityState>  m_visibilityCluster;
//    VoxelChunkTree<VoxelRenderChunk>    m_renderTree;
//    VoxelShapeChunkTree                 m_shapeTree;
//};