#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(std::uint32_t, g_NetPlayerClassId, 0x008BC15C);

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160,
         (std::uint32_t size, const char* sourceFile, int sourceLine));

CSF_FUNC(uint16_t*, __fastcall, NetPlayerDdl_BuildRecord, 0x007459B0,
         (void* self, void* edx, int a2));

static int __cdecl CreateGatheredNetPlayerDdl(int classId)
{
    g_NetPlayerClassId = classId;

    void* storage = AllocateGatheringStorage(
        60,
        CSF_STR(0x00867B48, ".\\PCGenerados\\NetPlayerDDL.cpp"),
        144);

    uint16_t* record = nullptr;
    if (storage)
        record = NetPlayerDdl_BuildRecord(storage, nullptr, classId);

    // Inline the original tail target 0x00661DC0, as in the verified clone.
    using SlotFn = int(__fastcall*)(void* self, void* edx, int arg);
    SlotFn initialize = reinterpret_cast<SlotFn>(
        CSF_AT(void**, record, 0)[8 / 4]);
    return initialize(record, nullptr, 0);
}

HOOK(0x00745AF0, CreateGatheredNetPlayerDdl, "auto");
