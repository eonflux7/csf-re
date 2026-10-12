#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(char, __fastcall, GfxParticleEmitters_ClearEmbeddedState, 0x00703710, (int* a1));
CSF_FUNC(int, __fastcall, RpPrtStd_ResetEmitterSubObject703670, 0x00703670, (void* self, void* edx));

// Clears the embedded emitter state at +0x20 of the object returned by GetValue_8BB738,
// then resets that same sub-object and returns the reset result.
static int __cdecl MissionInfo_PurgeRenderResources()
{
    int owner = GetValue_8BB738();
    uint8_t* owner_bytes = reinterpret_cast<uint8_t*>(owner);
    void* emitterSubObject = owner_bytes + 0x20;

    GfxParticleEmitters_ClearEmbeddedState(reinterpret_cast<int*>(emitterSubObject));
    return RpPrtStd_ResetEmitterSubObject703670(emitterSubObject, nullptr);
}

HOOK(0x006B16A0, MissionInfo_PurgeRenderResources, "auto");
