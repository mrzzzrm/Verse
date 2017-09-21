#include "VersePrototypeSystem.h"

#include <Deliberation/Platform/Application.h>
#include <Deliberation/Platform/ApplicationRuntime.h>

#include <Deliberation/Resource/PrototypesReloadedEvent.h>

#include "Emitter.h"
#include "VoxelMaterialPalettePrototype.h"
#include "WeaponPrototype.h"

VersePrototypeSystem::VersePrototypeSystem(World & world):
    PrototypeSystem(world)
{}

void VersePrototypeSystem::onCreated()
{
    m_manager->addPath<WeaponPrototype>(GameDataPath("Data/Prototypes/Weapons.json"));
    m_manager->addPath<VoxelMaterialPalettePrototype>(GameDataPath("Data/Prototypes/VoxelMaterialPalettes.json"));
    m_manager->addPath<VoxelMaterialPrototype>(GameDataPath("Data/Prototypes/VoxelMaterials.json"));
    m_manager->addPath<Emitter>(GameDataPath("Data/Prototypes/Emitters.json"));
    m_manager->reload();
}

void VersePrototypeSystem::reload()
{
    m_manager->reload();

    Application::get().runtime()->events()->publishEvent(PrototypesReloadedEvent());
}