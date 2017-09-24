#include "VoxelShredderSandbox.h"

#include <Deliberation/Core/Log.h>
#include <Deliberation/Core/Math/GLMUtils.h>
#include <Deliberation/Core/Math/Random.h>

#include <Deliberation/ECS/EntityPrototypeManager.h>
#include <Deliberation/ECS/Transform3DComponent.h>
#include <Deliberation/ECS/RigidBodyComponent.h>

#include <Deliberation/Platform/App.h>
#include <Deliberation/Platform/AppRuntime.h>

#include "VoxelShredder.h"
#include "VoxelClusterSplitSystem.h"
#include "VoxelObject.h"

VoxelShredderSandbox::VoxelShredderSandbox()
{
    activateInputLayer(0);
}

void VoxelShredderSandbox::onKeyPressed(KeyEvent & event)
{
    DELIBERATION_LOG_INNER_SCOPE("VoxelShredderSandbox");

    if (event.key() == Key_N)
    {
        Log->info("New scenario");
        for (auto & entity : m_segmentEntities)
        {
            if (entity.isValid()) entity.scheduleRemoval();
        }
        m_segmentEntities.clear();

        if (m_originalEntity.isValid()) m_originalEntity.scheduleRemoval();

        m_originalEntity = App::get().runtime()->entityPrototypeManager()->createEntity("Ship3");

        m_originalEntity.component<Transform3DComponent>().setPosition(glm::vec3(100.0f, 50.0f, -200.0f));
        m_originalEntity.component<Transform3DComponent>().setOrientation(glm::quat(glm::vec3(1.0f, 0.5f, 2.0f)));
    }

    if (event.key() == Key_M && m_originalEntity.isValid())
    {
        VoxelShredder().explode(m_originalEntity);


//        VoxelClusterSegment seg;
//        seg.voxels = {
//            glm::uvec3(1,0,0)
//        };
//        seg.llf = glm::uvec3(1,0,0);
//        seg.urb = glm::uvec3(1,0,0);
//        VoxelClusterSplitSystem::splitVoxelsOffEntity(m_originalEntity, seg);
//
//        seg.voxels = {
//            glm::uvec3(4,0,0)
//        };
//        seg.llf = glm::uvec3(4,0,0);
//        seg.urb = glm::uvec3(4,0,0);
//        VoxelClusterSplitSystem::splitVoxelsOffEntity(m_originalEntity, seg);
    }
}