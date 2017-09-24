#include "VoxelMaterialComponentPrototype.h"

#include <Deliberation/Resource/PrototypeManager.h>
#include <Deliberation/Platform/AppRuntime.h>
#include <Deliberation/Platform/App.h>

void VoxelMaterialComponentPrototype::updateComponent(const Entity & entity, VoxelMaterialComponent & voxelMaterialComponent)
{
    auto world = this->world();
    Assert(world);

    auto & prototypeManager = App::get().runtime()->prototypeManager();

    auto newVoxelMaterialPalette = prototypeManager->getOrCreatePrototype<VoxelMaterialPalettePrototype>(
        m_newJson["Palette"].get<std::string>());

    voxelMaterialComponent.setMaterialPalette(newVoxelMaterialPalette);
    voxelMaterialComponent.setCachedColorPalette(nullptr); //
}