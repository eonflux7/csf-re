#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, ParticleEmitters_AssignEntry, 0x006E1760,
         (void* self, void* edx, void* a2));

static void __fastcall ParticleEmitters_ReleaseEntry(
    void* self, void* /*edx*/, int index)
{
    if (index == -1)
        return;

    const uint32_t entryOffset = 60 * static_cast<uint32_t>(index);
    auto* entry = CSF_AT(uint8_t*, self, 0x74) + entryOffset;
    CSF_AT(uint32_t, entry, 0x38) =
        CSF_AT(uint32_t, entry, 0x38) - 1;

    entry = CSF_AT(uint8_t*, self, 0x74) + entryOffset;
    if (CSF_AT(uint32_t, entry, 0x38) != 0)
        return;

    void* ownedObject = CSF_AT(void*, entry, 0x1C);
    if (ownedObject)
    {
        using DestroyFunction = void (__fastcall*)(void*, void*, int);
        reinterpret_cast<DestroyFunction>(
            CSF_AT(void**, ownedObject, 0)[1])(ownedObject, nullptr, 1);

        entry = CSF_AT(uint8_t*, self, 0x74) + entryOffset;
        CSF_AT(void*, entry, 0x1C) = nullptr;
    }

    alignas(8) uint8_t emptyEntryBytes[0x3C]{};
    CSF_AT(uint32_t, emptyEntryBytes, 0x18) = 15;
    CSF_AT(float, emptyEntryBytes, 0x28) = 1.0f;
    CSF_AT(float, emptyEntryBytes, 0x2C) = 1.0f;

    entry = CSF_AT(uint8_t*, self, 0x74) + entryOffset;
    ParticleEmitters_AssignEntry(entry, nullptr, emptyEntryBytes);

    if (CSF_AT(uint32_t, emptyEntryBytes, 0x18) >= 0x10)
    {
        void* block = CSF_AT(void*, emptyEntryBytes, 0x04);
        if (block)
            Mem_FreeDebug(block);
    }
}

HOOK(0x006E1860, ParticleEmitters_ReleaseEntry, "auto");
