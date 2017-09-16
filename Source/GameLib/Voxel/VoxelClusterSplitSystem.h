#pragma once

#include <memory>
#include <set>

#include <Deliberation/ECS/System.h>

#include "GameLib.h"
#include "VoxelObjectModification.h"

class VoxelClusterSegment;
class VoxelObject;

class VoxelClusterSplitSystem : public System<VoxelClusterSplitSystem>
{
public:
    explicit VoxelClusterSplitSystem(World & world);

    void onEvent(const VoxelObjectModification & modification);

    void onCreated() override;

    static Entity splitVoxelsOffEntity(const Entity & originalEntity, const VoxelClusterSegment & segment);

    void onEntityGameUpdate(Entity & entity, const UpdateFrame & updateFrame) override;
    void onGameUpdate(const UpdateFrame & updateFrame) override;

private:
    std::set<std::shared_ptr<VoxelObject>> m_modifiedVoxelObjects;
};