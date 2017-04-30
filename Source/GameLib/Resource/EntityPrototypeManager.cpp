#include "EntityPrototypeManager.h"

#include <fstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/ECS/Components.h>

#include "AllegiancePrototype.h"
#include "CoriolisPrototype.h"
#include "EntityDesc.h"
#include "EntityPrototype.h"
#include "EquipmentPrototype.h"
#include "FlightControlConfigPrototype.h"
#include "HudProxyPrototype.h"
#include "NpcControllerPrototype.h"
#include "PlayerFlightControlPrototype.h"
#include "PlayerFlightControl.h"
#include "RigidBodyPrototype.h"
#include "VfxSystem.h"
#include "VoxelObjectPrototype.h"
#include "NpcBehaviourPrototype.h"

EntityPrototypeManager::EntityPrototypeManager(World & world):
    m_world(world)
{
    auto & voxelWorld = m_world.systemRef<VoxelWorld>();
    auto & vfxManager = m_world.systemRef<VfxSystem>().manager();

    registerComponentLoader<VoxelObjectPrototype>("VoxelObject", voxelWorld);
    registerComponentLoader<RigidBodyPrototype>("RigidBody");
    registerComponentLoader<CoriolisPrototype>("Coriolis");
    registerComponentLoader<EquipmentPrototype>("Equipment", vfxManager);
    registerComponentLoader<FlightControlConfigPrototype>("FlightControlConfig");
    registerComponentLoader<NpcControllerPrototype>("NpcController");
    registerComponentLoader<PlayerFlightControlPrototype>("PlayerFlightControl");
    registerComponentLoader<AllegiancePrototype>("Allegiance");
    registerComponentLoader<NpcBehaviourPrototype>("NpcBehaviour");
    registerComponentLoader<NpcBehaviourPrototype>("NpcBehaviour");
    registerComponentLoader<HudProxyPrototype>("HudProxy");
}

Entity EntityPrototypeManager::createEntity(const EntityDesc & desc)
{
    std::cout << "Creating entity '" << desc.entityName << "'" << std::endl;

    auto entity = m_world.createEntity(desc.entityName);

    for (const auto & prototypeName : desc.prototypeNames)
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

        std::ifstream prototypeFile(GameDataPath("Data/Prototypes/" + prototypeName + ".json"));
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

    // Apply secondary EntityDesc settings
    if (entity.hasComponent<RigidBodyComponent>())
    {
        auto & body = entity.component<RigidBodyComponent>().value();
        body->transform().setPosition(desc.position);
    }

    return entity;
}

template<typename T, typename ... Args>
void EntityPrototypeManager::registerComponentLoader(const std::string & name, Args &&... args)
{
    m_componentPrototypeLoaderByType[name] =
        [&] (const Json & jsonObj) { return std::make_shared<T>(jsonObj, std::forward<Args>(args)...); };
}
