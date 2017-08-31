#include "VoxReader.h"

#include <array>
#include <fstream>
#include <iostream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Log.h>
#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/StreamUtils.h>

#define VERBOSE 1

using namespace deliberation;

namespace
{
std::array<char, 4> VOX_ID = {'V', 'O', 'X', ' '};
std::array<char, 4> MAIN_CHUNK = {'M', 'A', 'I', 'N'};
std::array<char, 4> PACK_CHUNK = {'P', 'A', 'C', 'K'};
std::array<char, 4> SIZE_CHUNK = {'S', 'I', 'Z', 'E'};
std::array<char, 4> XYZI_CHUNK = {'X', 'Y', 'Z', 'I'};
std::array<char, 4> RGBA_CHUNK = {'R', 'G', 'B', 'A'};

u32 SUPPORTED_VERSION_NUMBER = 150;
u32 CHUNK_HEADER_SIZE = 12;
u32 MAX_NUM_MODELS_SUPPORTED = 16;

VoxReader::Palette DEFAULT_PALETTE = {
    0x00000000, 0xffffffff, 0xffccffff, 0xff99ffff, 0xff66ffff, 0xff33ffff,
    0xff00ffff, 0xffffccff, 0xffccccff, 0xff99ccff, 0xff66ccff, 0xff33ccff,
    0xff00ccff, 0xffff99ff, 0xffcc99ff, 0xff9999ff, 0xff6699ff, 0xff3399ff,
    0xff0099ff, 0xffff66ff, 0xffcc66ff, 0xff9966ff, 0xff6666ff, 0xff3366ff,
    0xff0066ff, 0xffff33ff, 0xffcc33ff, 0xff9933ff, 0xff6633ff, 0xff3333ff,
    0xff0033ff, 0xffff00ff, 0xffcc00ff, 0xff9900ff, 0xff6600ff, 0xff3300ff,
    0xff0000ff, 0xffffffcc, 0xffccffcc, 0xff99ffcc, 0xff66ffcc, 0xff33ffcc,
    0xff00ffcc, 0xffffcccc, 0xffcccccc, 0xff99cccc, 0xff66cccc, 0xff33cccc,
    0xff00cccc, 0xffff99cc, 0xffcc99cc, 0xff9999cc, 0xff6699cc, 0xff3399cc,
    0xff0099cc, 0xffff66cc, 0xffcc66cc, 0xff9966cc, 0xff6666cc, 0xff3366cc,
    0xff0066cc, 0xffff33cc, 0xffcc33cc, 0xff9933cc, 0xff6633cc, 0xff3333cc,
    0xff0033cc, 0xffff00cc, 0xffcc00cc, 0xff9900cc, 0xff6600cc, 0xff3300cc,
    0xff0000cc, 0xffffff99, 0xffccff99, 0xff99ff99, 0xff66ff99, 0xff33ff99,
    0xff00ff99, 0xffffcc99, 0xffcccc99, 0xff99cc99, 0xff66cc99, 0xff33cc99,
    0xff00cc99, 0xffff9999, 0xffcc9999, 0xff999999, 0xff669999, 0xff339999,
    0xff009999, 0xffff6699, 0xffcc6699, 0xff996699, 0xff666699, 0xff336699,
    0xff006699, 0xffff3399, 0xffcc3399, 0xff993399, 0xff663399, 0xff333399,
    0xff003399, 0xffff0099, 0xffcc0099, 0xff990099, 0xff660099, 0xff330099,
    0xff000099, 0xffffff66, 0xffccff66, 0xff99ff66, 0xff66ff66, 0xff33ff66,
    0xff00ff66, 0xffffcc66, 0xffcccc66, 0xff99cc66, 0xff66cc66, 0xff33cc66,
    0xff00cc66, 0xffff9966, 0xffcc9966, 0xff999966, 0xff669966, 0xff339966,
    0xff009966, 0xffff6666, 0xffcc6666, 0xff996666, 0xff666666, 0xff336666,
    0xff006666, 0xffff3366, 0xffcc3366, 0xff993366, 0xff663366, 0xff333366,
    0xff003366, 0xffff0066, 0xffcc0066, 0xff990066, 0xff660066, 0xff330066,
    0xff000066, 0xffffff33, 0xffccff33, 0xff99ff33, 0xff66ff33, 0xff33ff33,
    0xff00ff33, 0xffffcc33, 0xffcccc33, 0xff99cc33, 0xff66cc33, 0xff33cc33,
    0xff00cc33, 0xffff9933, 0xffcc9933, 0xff999933, 0xff669933, 0xff339933,
    0xff009933, 0xffff6633, 0xffcc6633, 0xff996633, 0xff666633, 0xff336633,
    0xff006633, 0xffff3333, 0xffcc3333, 0xff993333, 0xff663333, 0xff333333,
    0xff003333, 0xffff0033, 0xffcc0033, 0xff990033, 0xff660033, 0xff330033,
    0xff000033, 0xffffff00, 0xffccff00, 0xff99ff00, 0xff66ff00, 0xff33ff00,
    0xff00ff00, 0xffffcc00, 0xffcccc00, 0xff99cc00, 0xff66cc00, 0xff33cc00,
    0xff00cc00, 0xffff9900, 0xffcc9900, 0xff999900, 0xff669900, 0xff339900,
    0xff009900, 0xffff6600, 0xffcc6600, 0xff996600, 0xff666600, 0xff336600,
    0xff006600, 0xffff3300, 0xffcc3300, 0xff993300, 0xff663300, 0xff333300,
    0xff003300, 0xffff0000, 0xffcc0000, 0xff990000, 0xff660000, 0xff330000,
    0xff0000ee, 0xff0000dd, 0xff0000bb, 0xff0000aa, 0xff000088, 0xff000077,
    0xff000055, 0xff000044, 0xff000022, 0xff000011, 0xff00ee00, 0xff00dd00,
    0xff00bb00, 0xff00aa00, 0xff008800, 0xff007700, 0xff005500, 0xff004400,
    0xff002200, 0xff001100, 0xffee0000, 0xffdd0000, 0xffbb0000, 0xffaa0000,
    0xff880000, 0xff770000, 0xff550000, 0xff440000, 0xff220000, 0xff110000,
    0xffeeeeee, 0xffdddddd, 0xffbbbbbb, 0xffaaaaaa, 0xff888888, 0xff777777,
    0xff555555, 0xff444444, 0xff222222, 0xff111111};

} // namespace

std::vector<VoxReader::VoxelModel> VoxReader::read(const std::string & path)
{
    DELIBERATION_LOG_INNER_SCOPE("VoxReader");

#if VERBOSE
    Log->info("Loading '{}'", path);
#endif

    std::ifstream file(path, std::ios_base::binary);

    AssertM(file.is_open(), std::string("Couldn't open '") + path + "'");

    std::array<char, 4> id;
    file.read(id.data(), id.size());
    AssertM(id == VOX_ID, "Not a VOX file");

    u32 versionNumber = 0;
    file.read((char *)&versionNumber, sizeof(versionNumber));
    AssertM(versionNumber == SUPPORTED_VERSION_NUMBER, "Illegal version number");

    auto mainChunk = readChunkHeader(file);
    AssertM(mainChunk.type == MAIN_CHUNK, "No main chunk");

    auto packChunk = readChunkHeader(file);

    u32 numModels = 1;
    if (packChunk.type == PACK_CHUNK)
    {
        file.read((char *)&numModels, sizeof(numModels));
    }
    else
    {
        file.seekg(-CHUNK_HEADER_SIZE, std::ios_base::cur);
    }
    AssertM(
        numModels <= MAX_NUM_MODELS_SUPPORTED,
        "Number of models not supported");

    /**
     * Read XYZI/SIZE, i.e. Voxel, data
     */
    std::vector<VoxelModel> voxelModels(numModels);

    for (u32 m = 0; m < numModels; m++)
    {
        auto sizeChunk = readChunkHeader(file);
        AssertM(sizeChunk.type == SIZE_CHUNK, "No size chunk");

        file.read(
            (char *)&voxelModels[m].size.x, sizeof(voxelModels[m].size.x));
        file.read(
            (char *)&voxelModels[m].size.z, sizeof(voxelModels[m].size.z));
        file.read(
            (char *)&voxelModels[m].size.y, sizeof(voxelModels[m].size.y));

        auto xyziChunk = readChunkHeader(file);
        AssertM(xyziChunk.type == XYZI_CHUNK, "No xyzi chunk");
        AssertM(xyziChunk.numBytes % 4 == 0, "Invalid number of bytes");

        u32 numVoxels;
        file.read((char *)&numVoxels, sizeof(numVoxels));
        AssertM(numVoxels < 256 * 256 * 256, "Number of voxels too high");

        voxelModels[m].palettedVoxels.reserve(numVoxels);

        for (u32 v = 0; v < numVoxels; v++)
        {
            voxelModels[m].palettedVoxels.emplace_back(readVoxel(file));
        }
    }

    /**
     * Read pallete
     */
    Palette * palette = nullptr;
    Palette   paletteInFile;

    auto rgbaChunk = readChunkHeader(file);
    if (rgbaChunk.type == RGBA_CHUNK)
    {
#if VERBOSE
        Log->info("Custom palette");
#endif
        std::array<u8, 4> rgba;
        for (u32 c = 0; c < 255; c++)
        {
            file.read((char *)rgba.data(), rgba.size());
            paletteInFile[c + 1] = ((u32)rgba[0] << 0) | ((u32)rgba[1] << 8) |
                                   ((u32)rgba[2] << 16) | ((u32)rgba[3] << 24);
        }

        palette = &paletteInFile;
    }
    else
    {
        file.seekg(-CHUNK_HEADER_SIZE, std::ios_base::cur);
        palette = &DEFAULT_PALETTE;
    }

    /**
     *
     */
    std::vector<VoxelCluster<glm::vec3>> voxelClusters;
    voxelClusters.reserve(numModels);

    for (auto & voxelModel : voxelModels)
    {
        voxelClusters.emplace_back(voxelModel.size);

        auto & voxelCluster = voxelClusters.back();

        voxelModel.voxels.reserve(voxelModel.palettedVoxels.size());

        for (auto & voxel : voxelModel.palettedVoxels)
        {
            voxelModel.voxels.emplace_back(
                voxel.position, voxel.colorIndex, 200.0f);
        }

        // Generate palette
        voxelModel.palette.resize(palette->size());
        for (size_t i = 0; i < voxelModel.palette.size(); i++)
        {
            auto color = (*palette)[i];

            glm::u8vec4 colorv;
            colorv.x = (u8)(color & 0xFF);
            colorv.y = (u8)((color >> 8) & 0xFF);
            colorv.z = (u8)((color >> 16) & 0xFF);
            colorv.w = 255;

            voxelModel.palette[i] = colorv;
        }
    }

    return voxelModels;
}

VoxReader::Chunk VoxReader::readChunkHeader(std::ifstream & file)
{
    Chunk chunk;
    file.read(chunk.type.data(), chunk.type.size());
    file.read((char *)&chunk.numBytes, sizeof(chunk.numBytes));
    file.read((char *)&chunk.numBytesChildren, sizeof(chunk.numBytesChildren));
    return chunk;
}

VoxReader::PalettedVoxel VoxReader::readVoxel(std::ifstream & file)
{
    std::array<u8, 4> voxelData;
    file.read((char *)voxelData.data(), voxelData.size());

    PalettedVoxel voxel;
    voxel.position.x = voxelData[0];
    voxel.position.y = voxelData[2];
    voxel.position.z = voxelData[1];
    voxel.colorIndex = voxelData[3];

    return voxel;
}
