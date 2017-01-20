#pragma once

#in

class VoxelShapeTree final
{
public:
    VoxelShapeTree(const glm::uvec3 & size);

    void addVoxels(const std::vector<Voxel> & voxels);
    void removeVoxels(const std::vector<glm::uvec3> & voxels);

    bool lineCast(const Ray3D & ray, glm::uvec3 & voxel) const;

private:
    constexpr size_t NO_LEAF = std::numeric_limits<size_t>::max();

private:
    template<typename T>
    struct Subtree
    {
        void addVoxels(const std::vector<Voxel> & voxels);
        void removeVoxels(const std::vector<glm::uvec3> & voxels);
        void addVoxelToNode(size_t index, const Voxel & voxel);
        void removeVoxelFromNode(size_t index, const glm::uvec3 & voxel);
        void addVoxelToLeaf(size_t index, const Voxel & voxel);

        struct Node
        {
            glm::uvec3  llf;
            glm::uvec3  urb;
            Sphere      bounds;
            size_t      leaf = NO_LEAF;
            size_t      numHullVoxels = 0;
        };

        std::vector<Node>   nodes;
        std::vector<T>      leafs;
    };

private:
    glm::uvec3 &                        m_size;
    Subtree<std::shared_ptr<Subtree>>   m_tree;
};
