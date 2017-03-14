#include "EntityPrototypeManager.h"

#include <fstream>

#include <Deliberation/Core/Assert.h>
#include <Player/PlayerFlightControl.h>

#include "CoriolisPrototype.h"
#include "EntityPrototype.h"
#include "EquipmentPrototype.h"
#include "FlightControlConfigPrototype.h"
#include "NpcControllerPrototype.h"
#include "PlayerFlightControlPrototype.h"
#include "RigidBodyPrototype.h"
#include "VfxSystem.h"
#include "VoxelObjectPrototype.h"

EntityPrototypeManager::EntityPrototypeManager(World & world):
    m_world(world)
{
    auto & voxelWorld = m_world.system<VoxelWorld>();
    auto & vfxManager = m_world.system<VfxSystem>().manager();

    registerComponentLoader<VoxelObjectPrototype>("VoxelObject", voxelWorld);
    registerComponentLoader<RigidBodyPrototype>("RigidBody");
    registerComponentLoader<CoriolisPrototype>("Coriolis");
    registerComponentLoader<EquipmentPrototype>("Equipment", vfxManager);
    registerComponentLoader<FlightControlConfigPrototype>("FlightControlConfig");
    registerComponentLoader<NpcControllerPrototype>("NpcController");
    registerComponentLoader<PlayerFlightControlPrototype>("PlayerFlightControl");
}

Entity EntityPrototypeManager::createEntity(const std::vector<std::string> & prototypeNames,
                                            const std::string & entityName)
{
    auto entity = m_world.createEntity(entityName);

    for (const auto & prototypeName : prototypeNames)
    {
        {
            const auto iter = m_entityPrototypeByName.find(prototypeName);
            if (iter != m_entityPrototypeByName.end())
            {
                const auto & entityPrototype = iter->second;
                entityPrototype->applyToEntity(entity);

                continue;
            }
        }

        std::cout << "Loading EntityPrototype '" << prototypeName << "'" << std::endl;

        auto entityPrototype = std::make_shared<EntityPrototype>(m_world, prototypeName);

        std::ifstream prototypeFile("Data/Prototypes/" + prototypeName + ".json");
        Json entityPrototypeJson;
        prototypeFile >> entityPrototypeJson;

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
        entityPrototype->applyToEntity(entity);
    }

    return entity;
}

template<typename T, typename ... Args>
void EntityPrototypeManager::registerComponentLoader(const std::string & name, Args &&... args)
{
    m_componentPrototypeLoaderByType[name] =
        [&] (const Json & jsonObj) { return std::make_shared<T>(jsonObj, std::forward<Args>(args)...); };
}
