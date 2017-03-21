#pragma once

#include <memory>

#include <Deliberation/Core/HashUtils.h>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Scene/Mesh2.h>

#include "GameLib.h"
#include "R.h"

namespace deliberation
{
class Context;
}

class ResourceManager:
    public System<ResourceManager>
{
public:
    ResourceManager(World & world);

    const Mesh2 & mesh(ResourceId resourceId) const;

private:
    Context & m_context;

    std::unordered_map<size_t, std::shared_ptr<Mesh2>> m_meshByResourceId;
};