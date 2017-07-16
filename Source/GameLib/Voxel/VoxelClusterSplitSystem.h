#pragma once

#include <memory>
#include <set>

#include <Deliberation/ECS/System.h>

#include "GameLib.h"
#include "VoxelObjectModification.h"

class VoxelObject;

class VoxelClusterSplitSystem : public System<VoxelClusterSplitSystem>
{
public:
    VoxelClusterSplitSystem(World & world);

    void onEvent(const VoxelObjectModification & modification);

    void onCreated() override;

protected:
    void onEntityGameUpdate(Entity & entity, float seconds) override;
    void onGameUpdate(float seconds) override;

private:
    std::set<std::shared_ptr<VoxelObject>> m_modifiedVoxelObjects;
};