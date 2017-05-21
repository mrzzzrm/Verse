#pragma once

#include "Attachment.h"

struct VoxelLightDesc : AttachmentDesc
{
    float intensity = 0.0f;
};

class VoxelLight:
    public Attachment
{
public:
    VoxelLight(const VoxelLightDesc & desc);

protected:
    void onEnabled() override;
    void onDisabled() override;
    void onUpdate(float seconds) override;

private:
    VoxelLightDesc  m_desc;
    size_t          m_pointLight = 0;
};
