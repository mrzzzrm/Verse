#pragma once

#include <Deliberation/ECS/Component.h>

#include "GameLib.h"
#include "VoxelMaterialPalettePrototype.h"

class ColorPalette;

class VoxelMaterialComponent:
    public Component<VoxelMaterialComponent>
{
    DELIBERATION_COMPONENT_NAME("VoxelMaterial")

public:
    const std::shared_ptr<const VoxelMaterialPalettePrototype> & materialPalette() const { return m_materialPalette; }

    void setMaterialPalette(const std::shared_ptr<const VoxelMaterialPalettePrototype> & materialPalette)
    {
        m_materialPalette = materialPalette;
    }

    std::shared_ptr<const ColorPalette> cachedColorPalette() const { return m_cachedColorPalette; }

    void setCachedColorPalette(const std::shared_ptr<ColorPalette> & colorPalette)
    {
        m_cachedColorPalette = colorPalette;
    }

private:
    friend class VoxelMaterialComponentPrototype;

private:
    std::shared_ptr<const VoxelMaterialPalettePrototype> m_materialPalette;
    std::shared_ptr<ColorPalette> m_cachedColorPalette;
};