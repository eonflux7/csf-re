#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x004D0400: __thiscall (ecx = this), bound per packet.
CSF_FUNC(unsigned int, __fastcall, C_0083b668_FireShotAtSceneTarget, 0x004D0400, (C_0083b668* self, void* edx));

// Fires a shot at the scene target through the base class, then clears
// the state fields at +0x70 and +0x78 of C_00835228. Returns 0.
static int __fastcall C_00835228_FireShotAndResetState(C_00835228* self, void* /*edx*/)
{
    C_0083b668_FireShotAtSceneTarget(reinterpret_cast<C_0083b668*>(self), nullptr);
    self->field_0070 = 0;
    self->field_0078 = 0.0f;
    return 0;
}

HOOK(0x004A8D50, C_00835228_FireShotAndResetState, "auto");
