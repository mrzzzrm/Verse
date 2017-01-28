#pragma once

#include <array>
#include <fstream>
#include <vector>

#include <Deliberation/Core/IntTypes.h>

#include "GameLib.h"
#include "Voxel.h"
#include "VoxelCluster.h"

class VoxReader final
{
public:
    struct Chunk
    {
        std::array<char, 4> type;
        u32 numBytes = 0;
        u32 numBytesChildren = 0;
    };

    struct PalettedVoxel
    {
        glm::u8vec3 position;
        u8 colorIndex;
    };

    struct VoxelModel
    {
        glm::u32vec3 size;
        std::vector<PalettedVoxel> palettedVoxels;
        std::vector<Voxel> voxels;
    };

    using Palette = std::array<u32, 256>;

public:
    std::vector<VoxelModel> read(const std::string & path);

private:
    Chunk readChunkHeader(std::ifstream & file);
    PalettedVoxel readVoxel(std::ifstream & file);
};
