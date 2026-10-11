#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, SpawnPoints_InitTempId, 0x00652290, (uint32_t* a1));
CSF_FUNC(int, __fastcall, SpawnPoints_ApplyTempId, 0x00651D00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SpawnPoints_QueryHasEntry, 0x00647790, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SpawnPoints_ReleaseEntry, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SpawnPoints_ResolveEntryId, 0x00652070, (void* self, void* edx));

// Lazily resolves and caches this session's player entry id at +0xC4 (-1 = not yet resolved).
static int __fastcall MpSpawnPoints_GetPlayerEntryId(S_0073ecc0* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0xC4) == -1)
    {
        int tempId = 0;
        SpawnPoints_InitTempId(reinterpret_cast<uint32_t*>(&tempId));

        // Room 0xC bytes for the object the callees fill.
        alignas(8) uint8_t entryBytes[0xC];
        SpawnPoints_ApplyTempId(entryBytes, nullptr, tempId);

        const bool hasEntry = SpawnPoints_QueryHasEntry(entryBytes, nullptr) != 0;
        // The first dword of the entry object is passed on only when an entry exists.
        void* resolveArg = hasEntry ? *reinterpret_cast<void**>(entryBytes) : nullptr;

        CSF_AT(int, self, 0xC4) = SpawnPoints_ResolveEntryId(resolveArg, nullptr);
        SpawnPoints_ReleaseEntry(entryBytes, nullptr);
    }
    return CSF_AT(int, self, 0xC4);
}

HOOK(0x0073D0F0, MpSpawnPoints_GetPlayerEntryId, "auto");
