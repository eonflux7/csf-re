#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00655F00, __fastcall (reader, edx unused, slot pointer)
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));

// Original: 0x0073F7E0, __thiscall, ret 24 (six int arguments)
CSF_FUNC(char, __fastcall, CMenuMpStats_LatchStatsSnapshot, 0x0073F7E0,
         (CMenuMpStats* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7));

// Reads six consecutive values from the stream object, then latches them into the menu.
// The sixth value is read into the arg0 stack slot itself (as the original does at [ebp+8]).
// Original: 0x007478C0, __thiscall, ret 4
static char __fastcall CMenuMpStats_ReadAndLatchStatsSnapshot(CMenuMpStats* self, void* /*edx*/, C_008622e8* arg0)
{
    C_008622e8* reader = arg0;

    int a2;
    int a3;
    int a4;
    int a5;
    int a6;

    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&a2));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&a3));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&a4));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&a5));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&a6));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&arg0));

    return CMenuMpStats_LatchStatsSnapshot(self, nullptr, a2, a3, a4, a5, a6, reinterpret_cast<int>(arg0));
}

HOOK(0x007478C0, CMenuMpStats_ReadAndLatchStatsSnapshot, "auto");
