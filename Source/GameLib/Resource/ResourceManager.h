#pragma once

#include <memory>

#include <Deliberation/Core/HashUtils.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Scene/MeshData.h>

#include "GameLib.h"
#include "R.h"

namespace deliberation
{
class DrawContext;
}

class ResourceManager:
    public System<ResourceManager>
{
public:
    ResourceManager(World & world);

    const MeshData & mesh(ResourceId resourceId) const;
    const Program & program(ResourceId resourceId) const;

private:
    DrawContext &                                           m_drawContext;

    std::unordered_map<size_t, std::shared_ptr<MeshData>>  m_meshByResourceId;
    std::unordered_map<size_t, Program>                 m_programByResourceId;
};