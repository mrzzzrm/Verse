#include "VoxelLight.h"

#include <Deliberation/ECS/World.h>

#include <Deliberation/Scene/Lighting/PointLightRenderer.h>

#include "VoxelObject.h"

VoxelLight::VoxelLight(const VoxelLightDesc & desc):
    Attachment(desc),
    m_desc(desc)
{

}

void VoxelLight::onEnabled()
{
    auto & pointLightRenderer = m_entity.world().systemRef<PointLightSystem>().pointLightRenderer();
    auto & voxelObject = m_entity.component<VoxelObject>();

    auto intensity = voxelObject.data().voxelColor(voxel()) * m_desc.intensity;

    PointLight pointLight;
    pointLight.intensity = intensity;

    m_pointLight = pointLightRenderer.addPointLight(pointLight);

    voxelObject.setVoxelColor(voxel(), intensity);
}

void VoxelLight::onDisabled()
{
    auto & pointLightRenderer = m_entity.world().systemRef<PointLightSystem>().pointLightRenderer();

    pointLightRenderer.removePointLight(m_pointLight);
}

void VoxelLight::onUpdate(float /*seconds*/)
{
    auto & pointLightRenderer = m_entity.world().systemRef<PointLightSystem>().pointLightRenderer();
    auto & voxelObject = m_entity.component<VoxelObject>();

    auto & pointLight = pointLightRenderer.pointLight(m_pointLight);
    pointLight.position = voxelObject.pose().pointLocalToWorld(voxel());
}