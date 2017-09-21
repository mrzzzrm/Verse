#include "VoxelShredderSandbox.h"

#include <Deliberation/Core/Log.h>
#include <Deliberation/Core/Math/GLMUtils.h>
#include <Deliberation/Core/Math/Random.h>

#include <Deliberation/ECS/EntityPrototypeManager.h>
#include <Deliberation/ECS/Transform3DComponent.h>
#include <Deliberation/ECS/RigidBodyComponent.h>

#include <Deliberation/Platform/Application.h>
#include <Deliberation/Platform/ApplicationRuntime.h>

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

        m_originalEntity = Application::get().runtime()->entityPrototypeManager()->createEntity("Ship3");

        m_originalEntity.component<Transform3DComponent>().setPosition(glm::vec3(100.0f, 50.0f, -200.0f));
        m_originalEntity.component<Transform3DComponent>().setOrientation(glm::quat(glm::vec3(1.0f, 0.5f, 2.0f)));
    }

    if (event.key() == Key_M && m_originalEntity.isValid())
    {
        auto origin = m_originalEntity.component<Transform3DComponent>().transform().position();

        VoxelShredder shredder;
        auto entities = shredder.shred(m_originalEntity);

        for (auto & entity : entities)
        {
            auto & object = entity.component<VoxelObject>();
            auto & originalVoxelData = object.data();
            auto & splitDetector = object.data()->splitDetector();

            object.performSplitDetection();

            const auto & segments = splitDetector.segments();

            AssertM(
                !segments.empty() || originalVoxelData->numVoxels() == 0,
                entity.name() +
                ": there has to be one split, the object itself, by "
                    "definition. "
                    "Remaining voxels: " +
                std::to_string(originalVoxelData->numVoxels()));

            if (segments.size() <= 1)
            {
                m_segmentEntities.emplace_back(entity);
                continue;
            }

            for (size_t s = 0; s < segments.size(); s++)
            {
                if (s == splitDetector.splitContainingCrucialVoxel()) continue;
                auto entity2 = VoxelClusterSplitSystem::splitVoxelsOffEntity(entity, segments[s]);
                m_segmentEntities.emplace_back(entity2);
            }

            entity.scheduleRemoval();
        }

        for (auto & entity : m_segmentEntities) {
            auto & body = entity.component<RigidBodyComponent>().value();
            body->adaptShape();

            const auto direction = GLMSafeNormalize(body->transform().position() - origin);
            const auto intensity = RandomFloat(250.0f, 1000.0f) * body->mass() / 4.0f;

            body->applyCentralImpulse(direction * intensity);
            body->applyTorqueImpulse(0.04f * RandomVec3(1.0f, 2.0f) * intensity);
        }

        m_originalEntity.scheduleRemoval();

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