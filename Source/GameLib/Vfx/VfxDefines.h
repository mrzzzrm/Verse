#pragma once

#include <limits>

#include <Deliberation/Scene/Pipeline/RenderPhase.h>

#include "GameLib.h"

enum class VfxParticleOrientationType
{
    World = 0,
    ViewBillboard = 1,

    _Count_
};

using VfxMeshId = u32;

constexpr VfxMeshId INVALID_VFX_MESH_ID = std::numeric_limits<VfxMeshId>::max();
constexpr size_t INVALID_VFX_MESH_RENDER_BATCH_INDEX = std::numeric_limits<size_t>::max();

// Using a tuple auto-implements operator<()... so use it
using VfxBatchKey = std::tuple<
    size_t, // MeshID
    bool, // DLight
    RenderPhase,
    VfxParticleOrientationType>;