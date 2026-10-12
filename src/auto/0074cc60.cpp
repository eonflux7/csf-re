#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees bound by address (signatures per the packet).
CSF_FUNC(void*, __fastcall, ReadSerializedByte, 0x00655E20, (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, SpawnPoints_UpdateMultiplayerStats, 0x00747FB0, (void* self, void* edx, char a2, char a3));

// __thiscall(CMenuMpStats* this, C_008622e8* arg0) with ret 4.
static int __fastcall CMenuMpStats_ReadPlayerStatsAndUpdate(CMenuMpStats* self, void* /*edx*/, C_008622e8* arg0)
{
    // Original keeps its own copy of the source object pointer (edi) for both reads.
    C_008622e8* source = arg0;

    // First serialized byte goes into a 4-byte stack slot.
    alignas(4) char selectedByte[4] = {};
    ReadSerializedByte(source, nullptr, selectedByte);

    // Second serialized byte is written into the arg0 parameter slot itself.
    ReadSerializedByte(source, nullptr, reinterpret_cast<char*>(&arg0));

    // arg0 is reloaded here, so the low byte of the slot's new value is passed.
    return SpawnPoints_UpdateMultiplayerStats(self, nullptr,
                                              selectedByte[0],
                                              static_cast<char>(reinterpret_cast<uintptr_t>(arg0)));
}

HOOK(0x0074CC60, CMenuMpStats_ReadPlayerStatsAndUpdate, "auto");
