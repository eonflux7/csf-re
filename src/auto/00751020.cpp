#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(std::uint32_t, g_DdlObjectClassId, 0x008BC1CC);

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160,
         (std::uint32_t size, const char* sourceFile, int sourceLine));

CSF_FUNC(uint16_t*, __fastcall, NetChat_Construct, 0x00750FD0,
         (void* self, void* edx, int a2));

CSF_FUNC(int, __fastcall, NetChatDdl_InvokeRecordSlot, 0x00661DC0,
         (void* self, void* edx));

static int __cdecl CreateGatheredNetChatDdl(int a2)
{
    g_DdlObjectClassId = static_cast<std::uint32_t>(a2);

    void* gatheringStorage = AllocateGatheringStorage(
        0x24,
        CSF_STR(0x0086891C, ".\\PCGenerados\\NetChatDDL.cpp"),
        0x36);

    uint16_t* record = nullptr;
    if (gatheringStorage)
        record = NetChat_Construct(gatheringStorage, nullptr, a2);

    return NetChatDdl_InvokeRecordSlot(record, nullptr);
}

HOOK(0x00751020, CreateGatheredNetChatDdl, "auto");
