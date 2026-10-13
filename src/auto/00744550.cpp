#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// __thiscall(ecx = this, stack: selector, value), callee pops 8.
// selector 0 -> this+0x0C = value, returns value.
// selector 1 -> this+0x08 = value, returns 0.
// selector 2 -> this+0x04 = value, returns value.
// any other selector: no store, returns selector - 2.
static int __fastcall MpGameModes_SetHeaderOption(void* self, void* /*edx*/, int selector, int value)
{
    if (selector == 0)
    {
        CSF_AT(int, self, 0x0C) = value;
        return value;
    }
    if (selector == 1)
    {
        CSF_AT(int, self, 0x08) = value;
        return 0;
    }
    if (selector == 2)
    {
        CSF_AT(int, self, 0x04) = value;
        return value;
    }
    return selector - 2;
}

HOOK(0x744550, MpGameModes_SetHeaderOption, "auto");
