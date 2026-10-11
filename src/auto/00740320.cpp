#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x0064B250: returns non-zero when the menu stats can be stored.
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));

// Original 0x00751560: sets the flag of the sub-object embedded at +0x44.
CSF_FUNC(char, __fastcall, C_751560_SetFlag, 0x00751560, (void* self, void*));

// __thiscall with 3 stack args (ret 0xC).
static char __fastcall CMenuMpStats_StoreStatsAndSetFlag(CMenuMpStats* self, void* /*edx*/,
                                                         int firstValue, int secondValue, int thirdValue)
{
    char result = static_cast<char>(Sub_64B250(self, nullptr));
    if (result)
    {
        CSF_AT(int, self, 0x48) = firstValue;
        CSF_AT(int, self, 0x4C) = secondValue;
        CSF_AT(int, self, 0x50) = thirdValue;
        CSF_AT(int, self, 0x54) = 0;
        CSF_AT(int, self, 0x58) = 0;
        CSF_AT(int, self, 0x5C) = 0;
        return C_751560_SetFlag(reinterpret_cast<uint8_t*>(self) + 0x44, nullptr);
    }
    return result;
}

HOOK(0x00740320, CMenuMpStats_StoreStatsAndSetFlag, "auto");
