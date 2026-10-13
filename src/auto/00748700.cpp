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

// Fetches a spawn record into the caller-provided 8-byte buffer and returns a pointer to it.
CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));

// Commits the spawn record (this = record pointer) and returns a 64-bit result.
CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));

static int __fastcall MpSpawn_ProcessSelectedSlot(void* self, void* /*edx*/)
{
    alignas(8) uint8_t recordBytes[8];

    uint32_t* spawnRecord = MpSpawn_FetchSpawnRecord(reinterpret_cast<uint32_t*>(recordBytes));
    int64_t committed = MpSpawn_CommitSpawnRecord(reinterpret_cast<void*>(spawnRecord), nullptr);

    int32_t committedLow = static_cast<int32_t>(committed);
    int result = CSF_AT(int, self, 0x2E8) - committedLow;
    if (result < 0)
        return 0;
    return result;
}

HOOK(0x748700, MpSpawn_ProcessSelectedSlot, "auto");
