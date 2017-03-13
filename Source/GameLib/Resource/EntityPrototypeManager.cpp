#include "EntityPrototypeManager.h"

#include <fstream>

#include <Deliberation/Core/Assert.h>

#include "CoriolisPrototype.h"
#include "EntityPrototype.h"
#include "RigidBodyPrototype.h"
#include "VoxelObjectPrototype.h"

EntityPrototypeManager::EntityPrototypeManager(World & world):
    m_world(world)
{
    auto & voxelWorld = m_world.system<VoxelWorld>();

    registerComponentLoader<VoxelObjectPrototype>("VoxelObject", voxelWorld);
    registerComponentLoader<RigidBodyPrototype>("RigidBody");
    registerComponentLoader<CoriolisPrototype>("Coriolis");
}

Entity EntityPrototypeManager::createEntity(const std::string & prototypeName, const std::string & entityName)
{
    {
        const auto iter = m_entityPrototypeByName.find(prototypeName);
        if (iter != m_entityPrototypeByName.end())
        {
            const auto & entityPrototype = iter->second;
            entityPrototype->createEntity(entityName);
        }
    }

    auto entityPrototype = std::make_shared<EntityPrototype>(m_world, prototypeName);

    std::ifstream prototypeFile("Data/Prototypes/" + prototypeName + ".json");
    Json entityPrototypeJson;
    prototypeFile >> entityPrototypeJson;

    std::cout << "Loading EntityPrototype '" << prototypeName << "'" << std::endl;

    for (const auto & prototypeJson : entityPrototypeJson)
    {
        const auto & componentPrototypeType = prototypeJson["Component"].get<std::string>();

        std::cout << "  Component: '" << componentPrototypeType << "'" << std::endl;

        const auto iter = m_componentPrototypeLoaderByType.find(componentPrototypeType);
        Assert(iter != m_componentPrototypeLoaderByType.end(),
               "Can't load component type '" + componentPrototypeType + "'");

        const auto & componentPrototypeLoader = iter->second;

        auto componentPrototype = componentPrototypeLoader(prototypeJson);

        entityPrototype->addComponentPrototype(componentPrototype);
    }

    m_entityPrototypeByName.emplace(prototypeName, entityPrototype);

    return entityPrototype->createEntity(entityName);
}

template<typename T, typename ... Args>
void EntityPrototypeManager::registerComponentLoader(const std::string & name, Args &&... args)
{
    m_componentPrototypeLoaderByType[name] =
        [&] (const Json & jsonObj) { return std::make_shared<T>(jsonObj, std::forward<Args>(args)...); };
}
