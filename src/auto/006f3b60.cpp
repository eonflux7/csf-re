#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original: Mem_Free(void* block), cdecl, pushed as a single argument
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Original: GfxParticleEmitters_FinalizeSettings, __thiscall-style fastcall on the owned settings block
CSF_FUNC(int, __fastcall, GfxParticleEmitters_FinalizeSettings, 0x006DE360, (void* self, void* edx));

// Resets the emitter's per-update state, releases the owned settings block when it was flagged
// as owned (+0x34), clears the ownership flags, then stores the new settings pointer (a2) at +0x30.
static int __fastcall PrtStd_ResetEmitterStateAndStoreSettings(void* self, void* /*edx*/, int a2)
{
    if (CSF_AT(uint8_t, self, 0x34) != 0)
    {
        void* ownedSettings = CSF_AT(void*, self, 0x30);
        if (ownedSettings != nullptr)
        {
            GfxParticleEmitters_FinalizeSettings(ownedSettings, nullptr);
            Mem_Free(ownedSettings);
            CSF_AT(uint32_t, self, 0x30) = 0;
        }
        CSF_AT(uint32_t, self, 0x30) = 0;
        CSF_AT(uint8_t, self, 0x34) = 0;
        CSF_AT(uint8_t, self, 0x35) = 0;
    }

    CSF_AT(uint32_t, self, 0x2C) = 0;

    uint32_t flags = CSF_AT(uint32_t, self, 0x14) & 0xFFFFFFFDu;
    CSF_AT(uint32_t, self, 0x14) = flags;
    CSF_AT(uint32_t, self, 0x14) = flags | 1u;

    CSF_AT(int, self, 0x30) = a2;
    return a2;
}

HOOK(0x006F3B60, PrtStd_ResetEmitterStateAndStoreSettings, "auto");
