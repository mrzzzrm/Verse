#pragma once

#include <memory>
#include <set>

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class VoxelObject;

class VoxelClusterSplitSystem:
    public System<VoxelClusterSplitSystem>
{
public:
    VoxelClusterSplitSystem(World & world);

    void onVoxelObjectModified(const std::shared_ptr<VoxelObject> & object) { m_modifiedVoxelObjects.insert(object); }

protected:
    void onEntityUpdate(Entity & entity, float seconds) override;
    void onUpdate(float seconds) override;

private:
    std::set<std::shared_ptr<VoxelObject>> m_modifiedVoxelObjects;
};