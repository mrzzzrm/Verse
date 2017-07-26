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
constexpr size_t INVALID_VFX_RENDER_BATCH_INDEX = std::numeric_limits<size_t>::max();

using VfxBatchKey = std::tuple<size_t /*MeshId*/, RenderPhase, VfxParticleOrientationType>;