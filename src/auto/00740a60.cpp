#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bind as per packet (one name and signature per address).
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_7A2D60, 0x007A2D60, (void* self, void* edx, char a2));

// __thiscall method: ecx = this, edx unused, no stack args to pop (ret 0).
static char __fastcall CMenuMpStats_ProcessStatsUpdate(CMenuMpStats* self, void* /*edx*/) {
    char result = static_cast<char>(Sub_64B250(self, nullptr));
    if (result) {
        return sub_7A2D60(self, nullptr, 1);
    }
    return result;
}

HOOK(0x740A60, CMenuMpStats_ProcessStatsUpdate, "auto");
