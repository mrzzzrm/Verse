#include "VoxelMaterialComponentPrototype.h"

#include <Deliberation/Resource/PrototypeSystem.h>
#include <Deliberation/Resource/PrototypeManager.h>

void VoxelMaterialComponentPrototype::updateComponent(VoxelMaterialComponent & voxelMaterialComponent)
{
    auto oldPaletteName = m_json.empty() ? "" : m_json.value("Palette", "");
    auto newPaletteName = m_newJson.value("Palette", "");

    if (oldPaletteName == newPaletteName) return;

    auto & prototypeSystem = world().systemRef<PrototypeSystem>();
    auto & prototypeManager = prototypeSystem.manager();

    auto newVoxelMaterialPalette = prototypeManager->getOrCreatePrototype<VoxelMaterialPalettePrototype>(newPaletteName);

    voxelMaterialComponent.setMaterialPalette(newVoxelMaterialPalette);
    voxelMaterialComponent.setCachedColorPalette(nullptr); //
}