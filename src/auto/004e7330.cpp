#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Level test helper at 0x004E7090: __thiscall-style, one stack argument (ret 4).
CSF_FUNC(char, __fastcall, Level_IsEnabledFor, 0x004E7090, (void* self, void* edx, int a2));

// Returns the highest level (3..0) for which the probe succeeds, or -1 if none.
static int __fastcall C_0083e9d0_FindHighestEnabledLevel(C_0083e9d0* self, void* /*edx*/)
{
    if (Level_IsEnabledFor(self, nullptr, 3))
        return 3;
    if (Level_IsEnabledFor(self, nullptr, 2))
        return 2;
    if (Level_IsEnabledFor(self, nullptr, 1))
        return 1;
    if (Level_IsEnabledFor(self, nullptr, 0))
        return 0;
    return -1;
}

HOOK(0x004E7330, C_0083e9d0_FindHighestEnabledLevel, "auto");
