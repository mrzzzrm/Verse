#include "Emitter.h"

#include <Deliberation/Core/UpdateFrame.h>
#include <Deliberation/Resource/PrototypeManager.h>

#include "ResourceManager.h"
#include "VfxManager.h"
#include "VfxParticle.h"
#include "VfxSystem.h"

Emitter::Emitter(const std::weak_ptr<PrototypeManager> &prototypeManager)
    : AbstractPrototype(prototypeManager)
{
}

const std::shared_ptr<EmitterIntensityStrategy> & Emitter::intensity() const
{
    return m_intensity;
}

Pose3D & Emitter::pose() { return m_pose; }

void Emitter::setPose(const Pose3D & pose) { m_pose = pose; }

const std::vector<std::shared_ptr<const Emitter>> & Emitter::children() const
{
    return m_children;
}

void Emitter::addChild(std::shared_ptr<const Emitter> child)
{
    m_children.emplace_back(std::move(child));
}

void Emitter::updateInstance(
    VfxManager & vfxManager,
    EmitterInstance &        emitterInstance,
    EmitterInstanceContext & context,
    const UpdateFrame & updateFrame) const
{
    Assert(m_children.size() == context.children.size(), "Update Instances when changing Emitters!");

    for (size_t c = 0; c < m_children.size(); c++)
    {
        m_children[c]->updateInstance(vfxManager,
                                      emitterInstance, context.children[c], updateFrame);
    }

    if (!m_hasStrategies) return;

    if (m_meshRenderBatchIndex == INVALID_VFX_MESH_RENDER_BATCH_INDEX)
    {
        auto renderBatchKey = VfxBatchKey(m_meshID,
                                          false,
                                          RenderPhase::Alpha,
                                          m_orientation->orientationType());
        m_meshRenderBatchIndex =
            vfxManager.meshRenderer().getOrCreateBatchIndex(renderBatchKey);
    }

    auto timeAccumulator = 0.0f;

    Pose3D intermediatePose;

    while (!context.dead)
    {
        const auto timeStep =
            std::min(updateFrame.gameSeconds() - timeAccumulator, context.countdown);
        timeAccumulator += timeStep;
        context.countdown -= timeStep;

        if (context.countdown > 0.0f) break;

        intermediatePose = emitterInstance.m_basePose.interpolated(
            emitterInstance.m_targetPose, timeAccumulator / updateFrame.gameSeconds());

        intermediatePose = intermediatePose.poseLocalToWorld(m_pose);

        const auto position =
            intermediatePose.position() + m_placement->generatePosition();
        const auto birth =
            updateFrame.beginMicros()/1000 + (TimestampMillis)(timeAccumulator * 1000);

        auto particle = VfxParticle(
            position,
            intermediatePose.orientation() * m_velocity->generateVelocity() + emitterInstance.velocity(),
            birth,
            (DurationMillis)(m_lifetime->generateLifetime() * 1000));

        particle.meshRenderBatchIndex = m_meshRenderBatchIndex;

        particle.birthOrientation = m_orientation->generateOrientation();
        particle.axisRotation = m_rotation->generateRotation();

        const auto color = m_color->generate();
        particle.birthRGBA = color.birthRGBA;
        particle.deathRGBA = color.deathRGBA;

        const auto scale = m_size->generate();
        particle.birthScale = scale.birthScale;
        particle.deathScale = scale.deathScale;


        vfxManager.addParticle(particle);

        const auto nextInterval =
            m_intensity->generateInterval(emitterInstance, context);
        if (nextInterval == EmitterIntensityStrategy::NO_FURTHER_EMISSIONS)
        {
            emitterInstance.onEmitterDied();
            context.dead = true;
        }
        else
        {
            context.countdown += nextInterval;
        }
    }
}

void Emitter::onReload(const Json & json)
{
    Assert(json.is_object(), "Emitter JSON needs to be object:\n" + json.dump() + "\n");

    // Strategies
    const auto strategiesIter = json.find("Strategies");
    if (strategiesIter != json.end())
    {
        const auto & strategiesJson = *strategiesIter;

        // Lifetime
        const auto & lifetimeJson = strategiesJson["Lifetime"];
        if (lifetimeJson[0] == "RandomLifetime")
        {
            m_lifetime = std::make_shared<EmitterRandomLifetime>(lifetimeJson[1][0], lifetimeJson[1][1]);
        }
        else
        {
            Fail("No valid Lifetime: " + lifetimeJson.dump());
        }

        // Placement
        const auto & placementJson = strategiesJson["Placement"];
        if (placementJson[0] == "Fixed")
        {
            m_placement = std::make_shared<EmitterFixedPlacement>();
        }
        else
        {
            Fail("No valid Placement: " + placementJson.dump());
        }

        // Velocity
        const auto & velocityJson = strategiesJson["Velocity"];
        if (velocityJson[0] == "AnyDirection")
        {
            m_velocity = std::make_shared<EmitterAnyDirection>(velocityJson[1][0], velocityJson[1][1]);
        }
        else if (velocityJson[0] == "Cone")
        {
            m_velocity = std::make_shared<EmitterConeStrategy>(velocityJson[1][0], velocityJson[1][1], velocityJson[1][2]);
        }
        else if (velocityJson[0] == "Fixed")
        {
            m_velocity = std::make_shared<EmitterFixedDirection>(velocityJson[1][0], velocityJson[1][1]);
        }
        else
        {
            Fail("No valid Velocity: " + velocityJson.dump());
        }

        // Intensity
        const auto & intensityJson = strategiesJson["Intensity"];
        if (intensityJson[0] == "BurstIntensity")
        {
            m_intensity = std::make_shared<EmitterBurstIntensity>(intensityJson[1][0], intensityJson[1][1]);
        }
        else if (intensityJson[0] == "Noisy")
        {
            m_intensity = std::make_shared<EmitterNoisyIntensity>(intensityJson[1][0], intensityJson[1][1]);
        }
        else if (intensityJson[0] == "Fixed")
        {
            m_intensity = std::make_shared<EmitterFixedIntensity>(intensityJson[1][0].get<float>());
        }
        else
        {
            Fail("No valid Intensity: " + intensityJson.dump());
        }

        // Orientation
        const auto & orientationStrategy = strategiesJson["Orientation"];
        if (orientationStrategy[0] == "ViewBillboard")
        {
            m_orientation = std::make_shared<EmitterViewBillboardStrategy>();
        }
        else
        {
            Fail("No valid Orientation: " + orientationStrategy.dump());
        }

        // Rotation
        const auto & rotationJson = strategiesJson["Rotation"];
        if (rotationJson[0] == "None")
        {
            m_rotation = std::make_shared<EmitterNoRotation>();
        }
        else if (rotationJson[0] == "Billboard")
        {
            m_rotation = std::make_shared<EmitterBillboardRotation>(rotationJson[1][0], rotationJson[1][1]);
        }
        else
        {
            Fail("No valid Rotation: " + rotationJson.dump());
        }

        // Color
        const auto & colorJson = strategiesJson["Color"];
        if (colorJson[0] == "OverLifetime")
        {
            m_color = std::make_shared<EmitterColorOverLifetime>(colorJson[1][0], colorJson[1][1]);
        }
        else
        {
            Fail("No valid Color: " + colorJson.dump());
        }

        // Size
        const auto & sizeJson = strategiesJson["Size"];
        if (sizeJson[0] == "OverLifetime")
        {
            m_size = std::make_shared<EmitterSizeOverLifetime>(sizeJson[1][0], sizeJson[1][1]);
        }
        else if (sizeJson[0] == "Fixed")
        {
            m_size = std::make_shared<EmitterSizeOverLifetime>(sizeJson[1][0], sizeJson[1][0]);
        }
        else
        {
            Fail("No valid Size: " + sizeJson.dump());
        }

        // Mesh
        auto & world = prototypeManagerRef().world();
        auto & resourceManager = world.systemRef<ResourceManager>();
        auto & vfxManager = world.systemRef<VfxSystem>().manager();

        auto resourceToken = resourceManager.resourceToken<std::shared_ptr<MeshData>>(strategiesJson["Mesh"]);
        m_meshID = vfxManager->getOrCreateMeshId(resourceToken);

        m_meshRenderBatchIndex = INVALID_VFX_MESH_RENDER_BATCH_INDEX;

        m_hasStrategies = true;
    }

    // Children
    const auto childrenIter = json.find("Children");
    m_children.clear();
    if (childrenIter != json.end())
    {
        const auto & children = *childrenIter;

        for (const auto & child : children)
        {
            m_children.emplace_back(prototypeManagerRef().getOrCreatePrototype<Emitter>(child));
        }
    }
}