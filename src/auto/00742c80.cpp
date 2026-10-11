#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Last value passed in by the caller (stored to a global before allocation).
CSF_GLOBAL(int, g_NetCogibleDdlLastArg, 0x008BC128);

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160,
         (std::uint32_t size, const char* sourceFile, int sourceLine));

CSF_FUNC(uint16_t*, __fastcall, NetCogibleDdl_BuildRecord, 0x00742A90,
         (void* self, void* edx, int a2));

static int CreateGatheredNetCogibleDdl(int a2)
{
    g_NetCogibleDdlLastArg = a2;

    void* gatheringStorage = AllocateGatheringStorage(
        0x30,
        CSF_STR(0x00867940, ".\\PCGenerados\\NetCogibleDDL.cpp"),
        0x50);

    uint16_t* record = nullptr;
    if (gatheringStorage)
        record = NetCogibleDdl_BuildRecord(gatheringStorage, nullptr, a2);

    // Original tail-jumps to 0x661DC0: mov eax,[ecx]; push 0; call [eax+8]; ret.
    // Inlined here: vtable slot 2 (byte offset 8 / 4) called with this=record and one 0 argument.
    using SlotFn = int(__fastcall*)(void* self, void* edx, int arg);
    SlotFn slot2 = reinterpret_cast<SlotFn>(CSF_AT(void**, record, 0)[8 / 4]);
    return slot2(record, nullptr, 0);
}

HOOK(0x00742C80, CreateGatheredNetCogibleDdl, "auto");
