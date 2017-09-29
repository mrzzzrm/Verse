#include "DebugAttachmentSystem.h"

#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/ECS/RigidBodyComponent.h>
#include <Deliberation/ECS/Transform3DComponent.h>

#include "Equipment.h"

DebugAttachmentSystem::DebugAttachmentSystem(World & world)
    : Base(world, ComponentFilter::requires<Equipment>())
{
    activatePhases<FrameUpdatePhase>();
}

void DebugAttachmentSystem::onCreated()
{
    m_visibleNode =
        GetGlobal<RenderManager>()->renderer<DebugGeometryRenderer>()->addNode();
    m_obscuredNode =
        GetGlobal<RenderManager>()->renderer<DebugGeometryRenderer>()->addNode();

    DrawState obscuredDrawState;
    obscuredDrawState.setDepthState({DepthTest::Greater, DepthWrite::Enabled});
    obscuredDrawState.setBlendState(
        {BlendEquation::Add, BlendFactor::One, BlendFactor::One});
    m_obscuredNode->setDrawState(obscuredDrawState);
}

void DebugAttachmentSystem::onFrameUpdate(const UpdateFrame & updateFrame)
{
    auto & imGuiSystem = world().systemRef<ImGuiSystem>();

    if (!imGuiSystem.showView("Debug Attachments"))
    {
        m_visibleNode->setVisible(false);
        m_obscuredNode->setVisible(false);
        return;
    }
    m_visibleNode->setVisible(true);
    m_obscuredNode->setVisible(false);

    size_t currentAttachmentIndex = 0;

    for (const auto & entityEntry : m_entities)
    {
        if (!entityEntry.active) continue;

        const auto   entity = world().entityById(entityEntry.id);
        const auto & equipment = entity.component<Equipment>();

        for (const auto & attachment : equipment.attachments())
        {
            auto transform = Transform3D::fromPose(attachment->worldPose());
            transform.setScale(
                entity.component<Transform3DComponent>().transform().scale());

            updateSphere(
                m_visibleNode->sphere(currentAttachmentIndex),
                attachment,
                transform);
            updateSphere(
                m_obscuredNode->sphere(currentAttachmentIndex),
                attachment,
                transform);

            updatePose(
                m_visibleNode->pose(currentAttachmentIndex),
                attachment,
                transform);
            updatePose(
                m_obscuredNode->pose(currentAttachmentIndex),
                attachment,
                transform);

            currentAttachmentIndex++;
        }
    }

    m_numAllocatedPrimitives =
        std::max(currentAttachmentIndex, m_numAllocatedPrimitives);

    for (; currentAttachmentIndex < m_numAllocatedPrimitives;
         currentAttachmentIndex++)
    {
        m_visibleNode->pose(currentAttachmentIndex).setVisible(false);
        m_obscuredNode->pose(currentAttachmentIndex).setVisible(false);
        m_visibleNode->sphere(currentAttachmentIndex).setVisible(false);
        m_obscuredNode->sphere(currentAttachmentIndex).setVisible(false);
    }
}

void DebugAttachmentSystem::updateSphere(
    DebugSphereInstance &               sphere,
    const std::shared_ptr<Attachment> & attachment,
    const Transform3D &                 transform)
{
    sphere.setTransform(Transform3D::fromPose(attachment->worldPose()));
    sphere.setColor({0.3f, 1.0f, 0.3});
    sphere.setVisible(false);
    sphere.setRadius(transform.scale());
}

void DebugAttachmentSystem::updatePose(
    DebugPoseInstance &                 pose,
    const std::shared_ptr<Attachment> & attachment,
    const Transform3D &                 transform)
{
    auto transform2 = transform;

    transform2.setScale(transform.scale() * 20.0f);
    pose.setTransform(transform);
    pose.setVisible(true);
}
