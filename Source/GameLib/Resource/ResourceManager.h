#pragma once

#include <memory>

#include <Deliberation/Core/HashUtils.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Resource/ResourceToken.h>

#include <Deliberation/Scene/MeshData.h>

#include "GameLib.h"
#include "R.h"

namespace deliberation
{
class DrawContext;
}

class ResourceManager : public System<ResourceManager>
{
public:
    template<typename ResourceType> using ResourceLoader = std::function<ResourceType(const std::string &)>;

public:
    ResourceManager(World & world);

    // Old shit
    const MeshData & mesh(::ResourceId resourceId) const;
    const Program &  program(::ResourceId resourceId) const;


    //
    template<typename ResourceType>
        void setLoader(const ResourceLoader<ResourceType> & loader);

    template<typename ResourceType>
        ResourceToken resourceToken(const std::string & path);

    template<typename ResourceType>
        const ResourceType & resource(const std::string & path);

    template<typename ResourceType>
        const ResourceType & resource(const ResourceToken & token);

    template<typename ResourceType>
        void addResource(const std::string & path, ResourceType && resource);

protected:
    void registerBuildIns();

private:
    struct AbstractResourceContainer
    {
        virtual ~AbstractResourceContainer() = default;
    };

    template<typename ResourceType>
    struct ResourceContainer : AbstractResourceContainer
    {
        explicit ResourceContainer(const ResourceLoader<ResourceType> & loader):
            loader(loader)
        {}

        deliberation::ResourceId addResource(const std::string & path, ResourceType && resource);
        deliberation::ResourceId getOrLoadResource(const std::string & path);

        deliberation::ResourceId idCounter = 0;
        ResourceLoader<ResourceType> loader;
        std::unordered_map<deliberation::ResourceId, ResourceType> resourceById;
        std::unordered_map<std::string, deliberation::ResourceId> resourceIdByPath;
    };

private:
    template<typename ResourceType>
    ResourceContainer<ResourceType> & containerRef();

private:
    DrawContext & m_drawContext;

    // Old shit
    std::unordered_map<size_t, std::shared_ptr<MeshData>> m_meshByResourceId;
    std::unordered_map<size_t, Program>                   m_programByResourceId;

    //
    std::unordered_map<TypeID::value_t, std::shared_ptr<AbstractResourceContainer>>
        m_resourceContainersByTypeID;
};

#include "ResourceManager.inl"