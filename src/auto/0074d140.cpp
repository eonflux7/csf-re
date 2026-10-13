#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(std::uint32_t, g_NetGameClassId, 0x008BC178);

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160,
         (std::uint32_t size, const char* sourceFile, int sourceLine));

CSF_FUNC(uint16_t*, __fastcall, NetGame_Construct, 0x0074CF90,
         (void* self, void* edx, int a2));

static int NetGameDdl_CreateGatheredRecord(int a2)
{
    g_NetGameClassId = static_cast<std::uint32_t>(a2);

    void* gatheringStorage = AllocateGatheringStorage(
        0x30,
        CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"),
        116);

    uint16_t* record = nullptr;
    if (gatheringStorage)
        record = NetGame_Construct(gatheringStorage, nullptr, a2);

    // Original tail-jumps to 0x661DC0: mov eax,[ecx]; push 0; call [eax+8]; ret.
    // Inlined here: vtable slot 2 (byte offset 8 / 4) called with this=record and one 0 argument.
    using SlotFn = int(__fastcall*)(void* self, void* edx, int arg);
    SlotFn slot2 = reinterpret_cast<SlotFn>(CSF_AT(void**, record, 0)[8 / 4]);
    return slot2(record, nullptr, 0);
}

HOOK(0x0074D140, NetGameDdl_CreateGatheredRecord, "auto");
