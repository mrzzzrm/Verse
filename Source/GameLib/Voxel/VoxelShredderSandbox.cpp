#include "VoxelShredderSandbox.h"

#include <Deliberation/Core/Log.h>
#include <Deliberation/ECS/Systems/ApplicationSystem.h>

#include "VoxelShredder.h"

//VoxelShredderSandbox::VoxelShredderSandbox(World & world):
//    Base(world),
//    InputLayer(0)
//{}
//
//void VoxelShredderSandbox::onCreated()
//{
//    world().systemRef<ApplicationSystem>().input().addLayer(shared_from_this());
//}
//
//void VoxelShredderSandbox::onRemoved()
//{
//    world().systemRef<ApplicationSystem>().input().removeLayer(shared_from_this());
//}
//
//void VoxelShredderSandbox::onKeyPressed(KeyEvent & event)
//{
//    DELIBERATION_LOG_INNER_SCOPE("VoxelShredderSandbox");
//
//    if (event.key() == Key_N)
//    {
//        Log->info("New scenario");
//        for (auto & entity : m_segmentEntities)
//        {
//            if (entity.isValid()) entity.scheduleRemoval();
//        }
//        m_segmentEntities.clear();
//
//        if (m_originalEntity.isValid()) m_originalEntity.scheduleRemoval();
//    }
//
//    if (event.key() == Key_M && m_originalEntity.isValid())
//    {
//        VoxelShredder shredder;
//        m_segmentEntities = shredder.shred(m_originalEntity);
//        m_originalEntity.scheduleRemoval();
//    }
//}