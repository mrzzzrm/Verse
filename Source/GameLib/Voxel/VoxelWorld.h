#pragma once

#include <set>
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/Texture.h>

#include <Deliberation/ECS/System.h>

#include "VoxelClusterMarchingCubesTriangulation.h"
#include "VoxelDefines.h"
#include "VoxelObject.h"

namespace deliberation
{

class Camera3D;
class Context;
class PhysicsWorld;

}

class VoxelObject;
class VoxelObjectID;
class VoxelObjectModification;

class VoxelWorld final:
    public System<VoxelWorld>,
    public std::enable_shared_from_this<VoxelWorld>
{
public:
    VoxelWorld(World & world,
               const Camera3D & camera,
               const Texture & envMap);

    Context & context() const;
    const Camera3D & camera() const;
    const VoxelClusterMarchingCubesTriangulation & marchingCubesTriangulation() const;
    const Program & program() const;
    const Texture & envMap() const;

    void addVoxelObject(std::shared_ptr<VoxelObject> voxelObject);
    void removeVoxelObject(std::shared_ptr<VoxelObject> voxelObject);

    void addVoxelObjectModification(VoxelObjectModification && voxelObjectModification);

    void onCrucialVoxelDestroyed(VoxelObject & voxelObject);

protected:
    void onEntityAdded(Entity & entity) override;
    void onEntityRemoved(Entity & entity) override;
    void onEntityUpdate(Entity & entity, float seconds) override;
    void onUpdate(float seconds) override;
    void onRender() override;

private:
    Context &                                   m_context;
    PhysicsWorld &                              m_physicsWorld;
    const Camera3D &                            m_camera;
    Texture                                     m_envMap;
    VoxelClusterMarchingCubesTriangulation      m_marchingCubesTriangulation;
    Program                                     m_program;

    std::vector<std::shared_ptr<VoxelObject>>   m_objects;

    std::unordered_map<VoxelObjectWorldUID, std::shared_ptr<VoxelObject>>
                                                m_objectsByUID;

    u64                                         m_uidIncrementor = 0;

    std::vector<VoxelObjectModification>        m_objectModifications;
};