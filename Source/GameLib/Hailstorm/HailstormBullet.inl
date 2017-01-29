#pragma once

inline HailstormParticleID::HailstormParticleID(HailstormMeshID meshID,
                                            u32 renderBatchIndex,
                                            size_t physicsWorldIndex):
    meshID(meshID),
    renderBatchIndex(renderBatchIndex),
    physicsWorldIndex(physicsWorldIndex)
{

}

inline HailstormParticle::HailstormParticle(const glm::vec3 & origin,
                                        const glm::vec3 & velocity,
                                        float energy,
                                        TimestampMillis birth,
                                        DurationMillis lifetime,
                                        HailstormMeshID meshID,
                                        VoxelObjectWorldUID creator):
    id(meshID, 0, 0),
    origin(origin),
    velocity(velocity),
    energy(energy),
    birth(birth),
    lifetime(lifetime),
    creator(creator)
{
}