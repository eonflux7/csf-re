#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(std::uint32_t, g_RequiredObjectClassId, 0x008BC184);

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160,
         (std::uint32_t size, const char* sourceFile, int sourceLine));

CSF_FUNC(uint16_t*, __fastcall, NetPostman_Construct, 0x0074FD70,
         (void* self, void* edx, int a2));

CSF_FUNC(int, __fastcall, NetPostmanDdl_InvokeRecordSlot, 0x00661DC0,
         (void* self, void* edx));

static int __cdecl NetPostmanDdl_CreateGatheredRecord(int classId)
{
    g_RequiredObjectClassId = static_cast<std::uint32_t>(classId);

    void* storage = AllocateGatheringStorage(
        0x28,
        CSF_STR(0x008687A8, ".\\PCGenerados\\NetPostmanDDL.cpp"),
        0x54);

    uint16_t* record = nullptr;
    if (storage)
        record = NetPostman_Construct(storage, nullptr, classId);

    // Preserve the original forwarder, including its push 0 and virtual call.
    return NetPostmanDdl_InvokeRecordSlot(record, nullptr);
}

HOOK(0x0074FEF0, NetPostmanDdl_CreateGatheredRecord, "auto");
