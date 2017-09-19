#include "VoxelMaterialComponentPrototype.h"

#include <Deliberation/Resource/PrototypeSystem.h>
#include <Deliberation/Resource/PrototypeManager.h>

void VoxelMaterialComponentPrototype::updateComponent(const Entity & entity, VoxelMaterialComponent & voxelMaterialComponent)
{
    auto & prototypeSystem = world().systemRef<PrototypeSystem>();
    auto & prototypeManager = prototypeSystem.manager();

    auto newVoxelMaterialPalette = prototypeManager->getOrCreatePrototype<VoxelMaterialPalettePrototype>(
        m_newJson["Palette"].get<std::string>());

    voxelMaterialComponent.setMaterialPalette(newVoxelMaterialPalette);
    voxelMaterialComponent.setCachedColorPalette(nullptr); //
}