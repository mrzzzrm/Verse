#pragma once

#include "Attachment.h"

struct VoxelLightDesc : AttachmentDesc
{
    float intensity = 0.0f;
};

class VoxelLight : public Attachment
{
public:
    VoxelLight(const VoxelLightDesc & desc);

protected:
    void onEnabled() override;
    void onDisabled() override;
    void onGameUpdate(float seconds) override;

private:
    VoxelLightDesc m_desc;
    glm::vec3      m_color;
    size_t         m_pointLight = 0;
    float          m_noiseInput = 0;
};
