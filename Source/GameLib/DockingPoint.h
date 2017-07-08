#pragma once

#include "Attachment.h"

struct DockingPointDesc:
    AttachmentDesc
{};

class DockingPoint:
    public Attachment
{
public:
    DockingPoint(const DockingPointDesc & dockingPointDesc):
        Attachment(dockingPointDesc)
    {}
};