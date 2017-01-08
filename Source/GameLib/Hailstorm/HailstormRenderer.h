#pragma once

#include <memory>

#include <Deliberation/Core/LinearMap.h>

#include <Deliberation/Draw/Buffer.h>

#include <Deliberation/Scene/Mesh2.h>

#include "Hailstorm/HailstormBullet.h"
#include "Hailstorm/HailstormRenderBatch.h"

#include "GameLib.h"

namespace deliberation
{

class Camera3D;
class Context;

}

class HailstormRenderer final
{
public:
    HailstormRenderer(Context & context, const Camera3D & camera);

    Context & context() const;

    const Program & program();
    const Buffer & globalsBuffer() const;

    HailstormMeshID addMesh(const Mesh2 & mesh);

    void addBullet(HailstormBullet & bullet);
    void removeBullet(const HailstormBulletID & bullet);

    void update(float seconds);

private:
    Context &           m_context;
    const Camera3D &    m_camera;

    std::vector<std::unique_ptr<HailstormRenderBatch>>
                        m_batches;

    LayoutedBlob        m_globals;
    TypedBlobValueAccessor<glm::mat4>
                        m_viewProjectionGlobal;
    TypedBlobValueAccessor<uint32_t>
                        m_timeGlobal;

    Program             m_program;
    Buffer              m_globalsBuffer;
};