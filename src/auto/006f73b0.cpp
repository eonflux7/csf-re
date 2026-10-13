#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));
CSF_FUNC(int, __cdecl, _rpAtomicResyncInterpolatedSphere, 0x0058EB40, (int a1));

// Resyncs the provider frame's interpolated sphere when flag bit 2 is set at
// frame+0x50, then returns the frame address plus 28 (0x1C).
static int __fastcall C_00862900_ResyncFrameAndGetTail(C_00862900* self, void* /*edx*/)
{
    int frameObject = GfxFrames_GetProviderFrameObject((int)self->field_0004);
    if ((CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(frameObject), 0x50) & 2) != 0)
    {
        int providerFrameObject = GfxFrames_GetProviderFrameObject((int)self->field_0004);
        _rpAtomicResyncInterpolatedSphere(providerFrameObject);
    }
    return GfxFrames_GetProviderFrameObject((int)self->field_0004) + 28;
}

HOOK(0x006F73B0, C_00862900_ResyncFrameAndGetTail, "auto");
