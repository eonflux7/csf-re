#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));

static uint32_t GfxPrtStdParticles_GetEmitterPropertyAddress(void* emitter, int propertyId)
{
    const int offset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, CSF_AT(void*, emitter, 0x0C), 0x10), propertyId);

    return offset == -1
        ? 0
        : static_cast<uint32_t>(reinterpret_cast<uintptr_t>(emitter))
            + static_cast<uint32_t>(offset);
}

static void __fastcall GfxPrtStdParticles_CacheEmitterProperties(
    void* self, void* /*edx*/, void* emitter)
{
    if (emitter)
    {
        CSF_AT(uint32_t, self, 0x00) = GfxPrtStdParticles_GetEmitterPropertyAddress(emitter, 1);
        CSF_AT(uint32_t, self, 0x04) = GfxPrtStdParticles_GetEmitterPropertyAddress(emitter, 2);
        CSF_AT(uint32_t, self, 0x08) = GfxPrtStdParticles_GetEmitterPropertyAddress(emitter, 4);
        CSF_AT(uint32_t, self, 0x0C) = GfxPrtStdParticles_GetEmitterPropertyAddress(emitter, 5);
        CSF_AT(uint32_t, self, 0x10) = GfxPrtStdParticles_GetEmitterPropertyAddress(emitter, 6);
        CSF_AT(uint32_t, self, 0x14) = GfxPrtStdParticles_GetEmitterPropertyAddress(emitter, 8);
        CSF_AT(uint32_t, self, 0x24) = GfxPrtStdParticles_GetEmitterPropertyAddress(emitter, 16777223);
        CSF_AT(uint32_t, self, 0x18) = GfxPrtStdParticles_GetEmitterPropertyAddress(emitter, 9);
        CSF_AT(uint32_t, self, 0x1C) = GfxPrtStdParticles_GetEmitterPropertyAddress(emitter, 10);
        CSF_AT(uint32_t, self, 0x20) = GfxPrtStdParticles_GetEmitterPropertyAddress(emitter, 11);
    }
    else
    {
        CSF_AT(uint32_t, self, 0x00) = 0;
        CSF_AT(uint32_t, self, 0x04) = 0;
        CSF_AT(uint32_t, self, 0x08) = 0;
        CSF_AT(uint32_t, self, 0x0C) = 0;
        CSF_AT(uint32_t, self, 0x10) = 0;
        CSF_AT(uint32_t, self, 0x14) = 0;
        CSF_AT(uint32_t, self, 0x24) = 0;
        CSF_AT(uint32_t, self, 0x18) = 0;
        CSF_AT(uint32_t, self, 0x1C) = 0;
        CSF_AT(uint32_t, self, 0x20) = 0;
    }
}

HOOK(0x006F35C0, GfxPrtStdParticles_CacheEmitterProperties, "auto");
