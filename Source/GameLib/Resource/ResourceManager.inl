#include "ResourceManager.h"

#include <Deliberation/Core/StringUtils.h>

template<typename ResourceType>
void ResourceManager::setLoader(const ResourceLoader<ResourceType> & loader)
{
    m_resourceContainersByTypeID[TypeID::value<ResourceManager, ResourceType>()] =
                                         std::make_shared<ResourceContainer<ResourceType>>(loader);
}

template<typename ResourceType>
ResourceToken ResourceManager::resourceToken(const std::string & path)
{
    auto & container = containerRef<ResourceType>();

    auto id = container.getOrLoadResource(path);

    return ResourceToken(TypeID::value<ResourceManager, ResourceType>(), id);
}

template<typename ResourceType>
const ResourceType & ResourceManager::resource(const std::string & path)
{
    return resource<ResourceType>(resourceToken<ResourceType>(path));
}

template<typename ResourceType>
const ResourceType & ResourceManager::resource(const ResourceToken & token)
{
    auto & container = containerRef<ResourceType>();

    auto iter = container.resourceById.find(token.id());
    Assert(iter != container.resourceById.end(), "");

    return iter->second;
}

template<typename ResourceType>
void ResourceManager::addResource(const std::string & path, ResourceType && resource)
{
    auto & container = containerRef<ResourceType>();
    container.addResource(path, std::move(resource));
}

template<typename ResourceType>
ResourceManager::ResourceContainer<ResourceType> & ResourceManager::containerRef()
{
    const auto iter = m_resourceContainersByTypeID.find(TypeID::value<ResourceManager, ResourceType>());
    Assert(iter != m_resourceContainersByTypeID.end(), std::string("Couldn't find container for '") + typeid(ResourceType).name() + "'");
    return *std::static_pointer_cast<ResourceContainer<ResourceType>>(iter->second);
}

template<typename ResourceType>
deliberation::ResourceId ResourceManager::ResourceContainer<ResourceType>::addResource(const std::string & path,
                                                                                       ResourceType && resource)
{
    auto iter = resourceIdByPath.find(path);
    Assert(iter == resourceIdByPath.end(), "Resource '" + path + "' already exists");

    const auto id = idCounter;
    idCounter++;

    resourceIdByPath.emplace(path, id);
    resourceById.emplace(id, std::move(resource));

    return id;
}

template<typename ResourceType>
deliberation::ResourceId ResourceManager::ResourceContainer<ResourceType>::getOrLoadResource(const std::string & path) {
    auto iter = resourceIdByPath.find(path);
    if (iter == resourceIdByPath.end()) {
        auto resolvedPath = path;

        StringReplace(resolvedPath, "<GameDataPath>", GameDataPath());
        StringReplace(resolvedPath, "<DeliberationDataPath>", DeliberationDataPath());

        auto resource = loader(resolvedPath);
        return addResource(path, std::move(resource));
    }

    return iter->second;
}

