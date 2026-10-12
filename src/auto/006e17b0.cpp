#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, GfxParticleEmitters_ResetEntry, 0x006E1760, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static void __fastcall GfxParticleEmitters_ReleaseEntry(
    void* self, void* /*edx*/, int entryIndex)
{
    if (entryIndex == -1)
        return;

    const uint32_t entryOffset = 60 * static_cast<uint32_t>(entryIndex);

    --CSF_AT(uint32_t,
        CSF_AT(uint8_t*, self, 0x64) + entryOffset, 0x38);

    if (CSF_AT(uint32_t,
            CSF_AT(uint8_t*, self, 0x64) + entryOffset, 0x38) != 0)
        return;

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(sceneLoader, nullptr));

    const uint32_t objectHandle = CSF_AT(uint32_t,
        CSF_AT(uint8_t*, self, 0x64) + entryOffset, 0x1C);

    ((void(__fastcall*)(void*, void*, uint32_t))
        CSF_AT(void**, registry, 0)[0x2C / 4])(
            registry, nullptr, objectHandle);

    uint8_t* entry = CSF_AT(uint8_t*, self, 0x64) + entryOffset;
    alignas(8) uint8_t defaultEntryBytes[0x3C] = {};

    CSF_AT(uint32_t, defaultEntryBytes, 0x18) = 15;
    CSF_AT(uint32_t, defaultEntryBytes, 0x14) = 0;
    CSF_AT(uint8_t, defaultEntryBytes, 0x04) = 0;
    CSF_AT(uint32_t, defaultEntryBytes, 0x20) = 0;
    CSF_AT(uint32_t, defaultEntryBytes, 0x24) = 0;
    CSF_AT(float, defaultEntryBytes, 0x28) = 1.0f;
    CSF_AT(float, defaultEntryBytes, 0x2C) = 1.0f;
    CSF_AT(uint32_t, defaultEntryBytes, 0x1C) = 0;
    CSF_AT(uint32_t, defaultEntryBytes, 0x30) = 0;
    CSF_AT(uint32_t, defaultEntryBytes, 0x34) = 0;
    CSF_AT(uint32_t, defaultEntryBytes, 0x38) = 0;

    GfxParticleEmitters_ResetEntry(entry, nullptr, defaultEntryBytes);

    if (CSF_AT(uint32_t, defaultEntryBytes, 0x18) >= 0x10)
    {
        void* block = CSF_AT(void*, defaultEntryBytes, 0x04);
        if (block)
            Mem_FreeDebug(block);
    }
}

HOOK(0x006E17B0, GfxParticleEmitters_ReleaseEntry, "auto");
