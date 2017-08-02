#include <Voxel/VoxelMaterialPalettePrototype.h>
#include "VersePrototypeSystem.h"

#include "WeaponPrototype.h"

VersePrototypeSystem::VersePrototypeSystem(World & world):
    PrototypeSystem(world)
{}

void VersePrototypeSystem::onCreated()
{
    m_manager->addPath<WeaponPrototype>(GameDataPath("Data/Prototypes/Weapons.json"));
    m_manager->addPath<VoxelMaterialPalettePrototype>(GameDataPath("Data/Prototypes/VoxelMaterialPalettes.json"));
    m_manager->addPath<VoxelMaterialPrototype>(GameDataPath("Data/Prototypes/VoxelMaterials.json"));

    m_manager->reload();
}