#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPrtStdEmitterClassDestroy, 0x0057CD60, (int a1));
CSF_FUNC(int, __cdecl, RpPrtStdPClassDestroy, 0x0057FC60, (uint32_t* a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, GfxParticleEmitters_ClearBufferCollection, 0x006E08D0, (void* self, void* edx));
CSF_FUNC(char, __cdecl, RpPrtStd_ResetParticleBufferRange, 0x006E1710, (int a1, int a2));
CSF_FUNC(char, __fastcall, GfxParticleEmitters_ClearEmbeddedState, 0x00703710, (int* a1));
CSF_FUNC(void, __fastcall, GfxParticleEmitters_DestroyEmbeddedState, 0x007037C0, (void* self, void* edx));

static int __fastcall GfxParticleEmitters_DestroyEmitterClassesAndBuffers(
    void* self, void* /*edx*/)
{
    auto* bytes = static_cast<uint8_t*>(self);

    GfxParticleEmitters_ClearEmbeddedState(
        reinterpret_cast<int*>(bytes + 0x20));

    for (int offset = 0; offset < 16; offset += 4)
    {
        RpPrtStdEmitterClassDestroy(
            CSF_AT(int, CSF_AT(void*, self, 0x00), offset));
    }

    for (int offset = 0; offset < 16; offset += 4)
    {
        RpPrtStdPClassDestroy(
            CSF_AT(uint32_t*, CSF_AT(void*, self, 0x10), offset));
    }

    if (CSF_AT(int, self, 0x74))
    {
        RpPrtStd_ResetParticleBufferRange(
            CSF_AT(int, self, 0x74), CSF_AT(int, self, 0x78));
        if (CSF_AT(void*, self, 0x74))
            Mem_FreeDebug(CSF_AT(void*, self, 0x74));
    }
    CSF_AT(uint32_t, self, 0x74) = 0;
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(uint32_t, self, 0x7C) = 0;

    if (CSF_AT(int, self, 0x64))
    {
        RpPrtStd_ResetParticleBufferRange(
            CSF_AT(int, self, 0x64), CSF_AT(int, self, 0x68));
        if (CSF_AT(void*, self, 0x64))
            Mem_FreeDebug(CSF_AT(void*, self, 0x64));
    }
    CSF_AT(uint32_t, self, 0x64) = 0;
    CSF_AT(uint32_t, self, 0x68) = 0;
    CSF_AT(uint32_t, self, 0x6C) = 0;

    GfxParticleEmitters_ClearBufferCollection(bytes + 0x50, nullptr);
    if (CSF_AT(void*, self, 0x50))
    {
        Mem_Free(CSF_AT(void*, self, 0x50));
        CSF_AT(uint32_t, self, 0x50) = 0;
    }

    GfxParticleEmitters_ClearBufferCollection(bytes + 0x40, nullptr);
    if (CSF_AT(void*, self, 0x40))
    {
        Mem_Free(CSF_AT(void*, self, 0x40));
        CSF_AT(uint32_t, self, 0x40) = 0;
    }

    GfxParticleEmitters_DestroyEmbeddedState(bytes + 0x20, nullptr);

    if (CSF_AT(void*, self, 0x10))
    {
        Mem_Free(CSF_AT(void*, self, 0x10));
        CSF_AT(uint32_t, self, 0x10) = 0;
    }

    int result = CSF_AT(int, self, 0x00);
    if (result)
    {
        result = Mem_Free(CSF_AT(void*, self, 0x00));
        CSF_AT(uint32_t, self, 0x00) = 0;
    }
    return result;
}

HOOK(0x006E21C0, GfxParticleEmitters_DestroyEmitterClassesAndBuffers, "auto");
