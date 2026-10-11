#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, CMenuMpStats_ApplyStatsRow, 0x00740150, (void* self, void* edx, int a2, int a3, int a4, int a5));

// Reads four values from the DDL source object into out slots, then hands them to the menu.
// The fourth read overwrites the stack slot of arg0, and that updated value is what gets forwarded.
static bool __fastcall CMenuMpStats_ReadStatsRowAndApply(CMenuMpStats* self, void* /*edx*/, C_008622e8* arg0)
{
    C_008622e8* source = arg0;

    int a2 = 0;
    int a3 = 0;
    int a4 = 0;

    NetCogibleDdl_ReadIntoSlot(source, nullptr, reinterpret_cast<int>(&a2));
    NetCogibleDdl_ReadIntoSlot(source, nullptr, reinterpret_cast<int>(&a3));
    NetCogibleDdl_ReadIntoSlot(source, nullptr, reinterpret_cast<int>(&a4));
    NetCogibleDdl_ReadIntoSlot(source, nullptr, reinterpret_cast<int>(&arg0));

    return CMenuMpStats_ApplyStatsRow(self, nullptr, a2, a3, a4, reinterpret_cast<int>(arg0));
}

HOOK(0x007423D0, CMenuMpStats_ReadStatsRowAndApply, "auto");
