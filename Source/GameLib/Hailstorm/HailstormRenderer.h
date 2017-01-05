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
    using MeshID = u32;

public:
    HailstormRenderer(Context & context, Camera3D & camera);

    Context & context() const;

    MeshID addMesh(const Mesh2 & mesh);

    void addBullet(const HailstormBullet & bullet, MeshID meshID);
    void removeBullet(HailstormBulletID bulletID);

    void update(float seconds);

private:
    Context &   m_context;
    Camera3D &  m_camera;

    std::vector<std::unique_ptr<HailstormRenderBatch>>
                m_batches;

    LinearMap
};