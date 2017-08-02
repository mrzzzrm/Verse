#include "VoxelMaterialSystem.h"

#include "ColorPalette.h"
#include "VoxelObject.h"
#include "VoxelMaterialComponent.h"
#include "VoxelMaterialComponentPrototype.h"

VoxelMaterialSystem::VoxelMaterialSystem(World & world):
    Base(world, ComponentFilter::requires<VoxelMaterialComponent, VoxelObject>())
{}

void VoxelMaterialSystem::onEntityGameUpdate(Entity & entity, float seconds)
{
    auto & voxelObject = entity.component<VoxelObject>();

    if (!voxelObject.data()) return; // VoxelData will get added after component creation. This is awkward.... :( :(

    auto & voxelMaterialComponent = entity.component<VoxelMaterialComponent>();

    if (voxelMaterialComponent.cachedColorPalette() != voxelObject.data()->palette())
    {
        updateBrightnessScales(voxelObject, voxelMaterialComponent);
    }
}

void VoxelMaterialSystem::onEntityAdded(Entity & entity)
{
}

void VoxelMaterialSystem::updateBrightnessScales(VoxelObject & voxelObject,
                                           VoxelMaterialComponent & voxelMaterialComponent)
{
    auto & palette = voxelObject.data()->palette();
    auto & voxelMaterialPalette = voxelMaterialComponent.materialPalette();
    const auto & materialByPaletteColor = voxelMaterialPalette->materialByPaletteColor();

    for (const auto & pair : materialByPaletteColor)
    {
        const auto & paletteColor = pair.first;
        const auto & material = pair.second;

        const auto range = palette->getOrCreateIndicesByColor().equal_range(paletteColor);
        // TODO oh gosh, please get a proper format lib already
        Assert(std::distance(range.first, range.second) == 1, "Multiple indices with the same color " +
            std::to_string(paletteColor.x) + "," + std::to_string(paletteColor.y) + "," + std::to_string(paletteColor.z) + ": Brightness Scaling not supported");

        const auto index = range.first->second;

        palette->setBrightnessScale(index, material->brightnessScale());
    }

    voxelMaterialComponent.setCachedColorPalette(palette);
}