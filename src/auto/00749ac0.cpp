#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));
CSF_FUNC(void, __fastcall, SpawnPoints_ReleaseEntry, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SpawnPoints_ApplyTempId, 0x00651D00, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __cdecl, MpRecords_ResolveOwnerSlot, 0x00652290, (uint32_t* a1));

static int __fastcall MpSpawnPoints_InitializeDescriptor(C_008310b8* self, void* /*edx*/)
{
    // Stack objects, sized to the frame room the original gives them.
    alignas(8) uint8_t tempEntryBytes[0xC];
    void* tempEntry = tempEntryBytes;
    uint32_t ownerSlot = 0;

    C_008310b8_GetSelf(self, nullptr);
    CSF_AT(uint8_t, self, 0x20C) = 0;

    MpRecords_ResolveOwnerSlot(&ownerSlot);
    SpawnPoints_ApplyTempId(tempEntry, nullptr, static_cast<int>(ownerSlot));

    CSF_AT(uint32_t, self, 0x104) = 0;
    CSF_AT(uint16_t, self, 0x10C) = 0;
    CSF_AT(uint8_t, self, 0) = 0;
    CSF_AT(uint8_t, self, 1) = 0;
    CSF_AT(uint32_t, self, 0x108) = 0;

    SpawnPoints_ReleaseEntry(tempEntry, nullptr);
    return reinterpret_cast<int>(self);
}

HOOK(0x00749AC0, MpSpawnPoints_InitializeDescriptor, "auto");
