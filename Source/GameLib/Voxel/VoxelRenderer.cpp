#include "VoxelRenderer.h"

#include <Deliberation/Scene/Pipeline/RenderPhase.h>

VoxelRenderer::VoxelRenderer(
    RenderManager & renderManager, const Texture & envMap)
    : SingleNodeRenderer(renderManager, RenderPhase::GBuffer, "VoxelObjects")
{
}

void VoxelRenderer::addVoxelObject(std::shared_ptr<VoxelObject> voxelObject)
{
    VoxelObjectID id;
    id.worldUID = m_uidIncrementor;

    voxelObject->setId(id);
    m_uidIncrementor++;

    m_objects.emplace_back(voxelObject);
}

void VoxelRenderer::removeVoxelObject(std::shared_ptr<VoxelObject> voxelObject)
{
    const auto iter =
        std::find(m_objects.begin(), m_objects.end(), voxelObject);
    Assert(iter != m_objects.end(), "");

    m_objects.erase(iter);
}

void VoxelRenderer::render()
{
    for (auto & object : m_objects)
        object->render();
}
