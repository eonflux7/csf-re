#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(std::uint32_t, g_NetProyectilClassId, 0x008BC160);

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160,
         (std::uint32_t size, const char* sourceFile, int sourceLine));

CSF_FUNC(uint16_t*, __fastcall, NetProyectilDdl_BuildRecord, 0x00747AE0,
         (void* self, void* edx, int a2));

CSF_FUNC(int, __fastcall, GatheringStorage_InvokeCompletion, 0x00661DC0,
         (void* self, void* edx));

static int __cdecl CreateGatheredNetProyectilDdl(int classId)
{
    g_NetProyectilClassId = static_cast<std::uint32_t>(classId);

    void* storage = AllocateGatheringStorage(
        40,
        CSF_STR(0x00867ECC, ".\\PCGenerados\\NetProyectilDDL.cpp"),
        62);

    uint16_t* record = nullptr;
    if (storage)
        record = NetProyectilDdl_BuildRecord(storage, nullptr, classId);

    return GatheringStorage_InvokeCompletion(record, nullptr);
}

HOOK(0x00747BF0, CreateGatheredNetProyectilDdl, "auto");
