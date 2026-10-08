#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int (__cdecl* const RwFrameGetLTM)(int) =
    reinterpret_cast<int (__cdecl*)(int)>(CSF_ADDR_FN(0x00623720));
static int (__cdecl* const RwFrameTransform)(int, int, int) =
    reinterpret_cast<int (__cdecl*)(int, int, int)>(CSF_ADDR_FN(0x00623860));
static int (__cdecl* const RwFrameRemoveChild)(int) =
    reinterpret_cast<int (__cdecl*)(int)>(CSF_ADDR_FN(0x00623750));

static int __fastcall C_00860700_DetachFramePreservingWorldTransform(
    C_00860700* self, void* /*edx*/)
{
    const uint32_t frame = self->field_000c;
    if (CSF_AT(uint32_t, reinterpret_cast<void*>(frame), 0x04) != 0)
    {
        const int worldTransform = RwFrameGetLTM(static_cast<int>(frame));
        RwFrameTransform(static_cast<int>(self->field_000c), worldTransform, 0);
        return RwFrameRemoveChild(static_cast<int>(self->field_000c));
    }
    return static_cast<int>(frame);
}

HOOK(0x006BF280, C_00860700_DetachFramePreservingWorldTransform, "auto");
