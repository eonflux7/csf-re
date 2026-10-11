#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Rebuilds the rotation state after the yaw changes.
// Original: 0x006AE570, __thiscall, no stack args.
CSF_FUNC(void, __fastcall, Rotation_RebuildMatrix, 0x006AE570, (void* self, void* edx));

// Adds a yaw delta to the rotation value at +0x000, then rebuilds the rotation.
static void __fastcall Rotation_ApplyYawDelta(S_006ae570* self, void* /*edx*/, float yawDelta)
{
    self->field_0000 = yawDelta + self->field_0000;
    (void)CSF_ADDR_VT(0x006AE570); // packet lists this address as a vtable reference; see notes
    Rotation_RebuildMatrix(self, nullptr);
}

HOOK(0x006AE6B0, Rotation_ApplyYawDelta, "auto");
