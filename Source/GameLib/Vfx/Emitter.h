#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/Resource/AbstractPrototype.h>

#include "EmitterColorStrategy.h"
#include "EmitterInstance.h"
#include "EmitterIntensityStrategy.h"
#include "EmitterLifetimeStrategy.h"
#include "EmitterPlacementStrategy.h"
#include "EmitterOrientationStrategy.h"
#include "EmitterRotationStrategy.h"
#include "EmitterSizeStrategy.h"
#include "EmitterVelocityStrategy.h"
#include "GameLib.h"
#include "VfxDefines.h"

class VfxManager;

class Emitter final:
    public AbstractPrototype
{
public:
    explicit Emitter(const std::weak_ptr<PrototypeManager> &prototypeManager = {});

    const std::shared_ptr<EmitterIntensityStrategy> & intensity() const;

    Pose3D & pose();
    void     setPose(const Pose3D & pose);

    const std::vector<std::shared_ptr<const Emitter>> & children() const;

    void addChild(std::shared_ptr<const Emitter> child);

    void updateInstance(
        VfxManager & vfxManager,
        EmitterInstance &        emitterInstance,
        EmitterInstanceContext & context,
        const UpdateFrame & updateFrame) const;

protected:
    void onReload(const Json & json) override;

private:
    VfxMeshId                                 m_meshID = INVALID_VFX_MESH_ID;
    mutable size_t                            m_meshRenderBatchIndex = INVALID_VFX_MESH_RENDER_BATCH_INDEX;
    // Does it emit particles itself or is it only a collection of children?
    bool                                      m_hasStrategies = false;
    std::shared_ptr<EmitterVelocityStrategy>  m_velocity;
    std::shared_ptr<EmitterOrientationStrategy>  m_orientation;
    std::shared_ptr<EmitterPlacementStrategy> m_placement;
    std::shared_ptr<EmitterIntensityStrategy> m_intensity;
    std::shared_ptr<EmitterLifetimeStrategy>  m_lifetime;
    std::shared_ptr<EmitterColorStrategy>     m_color;
    std::shared_ptr<EmitterSizeStrategy>      m_size;
    std::shared_ptr<EmitterRotationStrategy>  m_rotation;

    Pose3D m_pose;

    std::vector<std::shared_ptr<const Emitter>> m_children;
};