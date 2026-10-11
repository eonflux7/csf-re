#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Resolves a pointer-to-slot from a record reference; the result's first dword is the owner id.
CSF_FUNC(uint32_t*, __cdecl, MpRecords_ResolveOwnerSlot, 0x00652290, (uint32_t* a1));

static int __fastcall MpSpawnPoints_GetRecordOwnerId(void* self, void* /*edx*/)
{
    void* recordRef = self;
    uint32_t* ownerSlot = MpRecords_ResolveOwnerSlot(reinterpret_cast<uint32_t*>(&recordRef));
    return static_cast<int>(*ownerSlot);
}

HOOK(0x0073CD90, MpSpawnPoints_GetRecordOwnerId, "auto");
