#pragma once

#include <vector>

#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/System.h>

#include <Deliberation/Platform/Input.h>
#include <Deliberation/Platform/InputLayer.h>

#include "AbstractBehaviour.h"

//class VoxelShredderSandbox:
//    public AbstractBehaviour
//{
//public:
//    explicit VoxelShredderSandbox(World & world);
//
//    void onCreated() override;
//    void onRemoved() override;
//
//protected:
//    void onKeyPressed(KeyEvent & event) override;
//
//private:
//    Entity m_originalEntity;
//    std::vector<Entity> m_segmentEntities;
//};