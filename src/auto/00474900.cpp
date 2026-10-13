#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_0082e0f4_InitializeSpawnRecord, 0x004164B0, (C_0082e0f4* self, void* , int ownerId));
CSF_FUNC(int, __fastcall, C_0082e0f4_RegisterSpawnRecord, 0x00474380, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_0082e0f4_AllocSpawnRecord(C_0082e0f4* self, void* /*edx*/)
{
    C_0082e0f4* record = reinterpret_cast<C_0082e0f4*>(CoreMemory_AllocUntagged(116));
    if (record)
    {
        uint32_t* spawnRecord = C_0082e0f4_InitializeSpawnRecord(record, nullptr, -1);
        C_0082e0f4_RegisterSpawnRecord(self, nullptr, reinterpret_cast<int>(spawnRecord));
        return spawnRecord;
    }

    C_0082e0f4_RegisterSpawnRecord(self, nullptr, 0);
    return nullptr;
}

HOOK(0x00474900, C_0082e0f4_AllocSpawnRecord, "auto");
