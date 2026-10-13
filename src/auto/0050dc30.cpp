#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// __thiscall void(char flag, float value): two stack args, so ret 8.
// Written as __fastcall with a dummy edx, as the convention requires.
static void __fastcall GlobalState_ApplyFloat(void* self, void* /*edx*/, char flag, float value)
{
    // Only applies when the lock byte at +0xD0 is clear.
    if (CSF_AT(char, self, 0xD0) != 0)
        return;

    CSF_AT(char, self, 0x94) = flag;
    if (flag != 0)
    {
        CSF_AT(float, self, 0x98) = value;
        if (value < g_fZero)
            CSF_AT(uint32_t, self, 0x98) = 0;
    }
}

HOOK(0x0050DC30, GlobalState_ApplyFloat, "auto");
