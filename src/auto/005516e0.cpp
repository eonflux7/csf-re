#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Binds (callee signatures per the packet)
CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));
CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));

// 0x5516E0: fills an 8-byte scratch record, then passes the returned record
// pointer as `this` to the commit routine and returns its 64-bit result.
static int64_t __cdecl MpSpawn_QueryAndCommitSpawn()
{
    uint32_t scratchRecord[2] = {0, 0};
    uint32_t* spawnRecord = MpSpawn_FetchSpawnRecord(scratchRecord);
    return MpSpawn_CommitSpawnRecord(spawnRecord, nullptr);
}

HOOK(0x5516E0, MpSpawn_QueryAndCommitSpawn, "auto");
