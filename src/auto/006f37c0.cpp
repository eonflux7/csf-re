#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, sub_6CA410, 0x006CA410, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxParticleEmitters_FinalizeSettings, 0x006DE360, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(int, __fastcall, GfxParticleEmitters_ResolveParticleResource, 0x006E15F0, (void* self, void* edx, C_0082d410* a2));
CSF_FUNC(int, __fastcall, ParticleEmitters_AddEmitterIfNotPresent, 0x006E2B80, (void* self, void* edx, C_0082d410* a2));

// Refreshes the atomic particle emitter state of this object from the given setup block (a2).
static int __fastcall C_00862748_UpdateAtomicParticleEmitter(C_00862748* self, void* /*edx*/, int a2)
{
    sub_6CA410(self, nullptr, a2);

    if (self->field_0034)
    {
        S_006dcf90* emitterSettings = reinterpret_cast<S_006dcf90*>(self->field_0030);
        if (emitterSettings)
        {
            GfxParticleEmitters_FinalizeSettings(emitterSettings, nullptr);
            Mem_Free(emitterSettings);
            self->field_0030 = 0;
        }
        self->field_0030 = 0;
        self->field_0034 = 0;
        self->field_0035 = 0;
    }

    // Emitter sub-block lives at a2 + 4; flag byte is at a2 + 0x4C.
    C_0082d410* emitterBlock = reinterpret_cast<C_0082d410*>(a2 + 4);
    uint8_t usesEmitterList = *reinterpret_cast<uint8_t*>(a2 + 0x4C);

    if (usesEmitterList)
    {
        self->field_002c = 1;
        GetValue_8BB738();
        int result = ParticleEmitters_AddEmitterIfNotPresent(
            reinterpret_cast<void*>(GetValue_8BB738()), nullptr, emitterBlock);
        self->field_0038 = static_cast<uint32_t>(result);
        return result;
    }
    else
    {
        self->field_002c = 0;
        int result = GfxParticleEmitters_ResolveParticleResource(
            reinterpret_cast<void*>(GetValue_8BB738()), nullptr, emitterBlock);
        self->field_0030 = static_cast<uint32_t>(result);
        return result;
    }
}

HOOK(0x006F37C0, C_00862748_UpdateAtomicParticleEmitter, "auto");
