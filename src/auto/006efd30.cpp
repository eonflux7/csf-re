#include <cstdint>
#include "globals.h"
#include "hook.h"

// Forwards to the emitter-state reset, passing the sub-object at this+0x24.
CSF_FUNC(int, __fastcall, PrtStd_ResetEmitterStateAndStoreSettings, 0x006F3B60, (void* self, void* /*edx*/, int a2));

static int __fastcall ParticlePtank_ResetEmitterFromSubobject(void* self, void* edx, int a2)
{
    // add ecx, 0x24: the callee's this is the embedded sub-object at +0x24.
    void* emitterSettings = static_cast<uint8_t*>(self) + 0x24;
    return PrtStd_ResetEmitterStateAndStoreSettings(emitterSettings, edx, a2);
}

HOOK(0x006EFD30, ParticlePtank_ResetEmitterFromSubobject, "auto");
