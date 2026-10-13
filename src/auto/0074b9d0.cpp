#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(std::uint32_t, g_NetGhostClassId, 0x008BC174);

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160,
         (std::uint32_t size, const char* sourceFile, int sourceLine));

CSF_FUNC(uint16_t*, __fastcall, NetGhost_Construct, 0x0074B850,
         (void* self, void* edx, int a2));

CSF_FUNC(int, __fastcall, NetGhostDdl_FinalizeGatheredRecord, 0x00661DC0,
         (void* self, void* edx));

static int __cdecl NetGhostDdl_CreateGatheredRecord(int a2)
{
    g_NetGhostClassId = static_cast<std::uint32_t>(a2);

    void* storage = AllocateGatheringStorage(
        0x28,
        CSF_STR(0x008680F8, ".\\PCGenerados\\NetGhostDDL.cpp"),
        0x50);

    uint16_t* record = nullptr;
    if (storage)
        record = NetGhost_Construct(storage, nullptr, a2);

    return NetGhostDdl_FinalizeGatheredRecord(record, nullptr);
}

HOOK(0x0074B9D0, NetGhostDdl_CreateGatheredRecord, "auto");
