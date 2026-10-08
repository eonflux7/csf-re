#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x00542DB0 (__thiscall, ret 4)
// If the current state (this+0x2D8) is 1, sets two flags (this+0x2C8, this+0x2CC)
// to 1 and advances the state to 2. Always returns 1.
static int __fastcall CMenuControls_TryAdvanceState(void* self, void* /*edx*/, int /*a2*/)
{
    const int result = 1;
    if (CSF_AT(int, self, 0x2d8) == result)
    {
        CSF_AT(int, self, 0x2c8) = result;
        CSF_AT(int, self, 0x2cc) = result;
        CSF_AT(int, self, 0x2d8) = 2;
    }
    return result;
}

HOOK(0x542DB0, CMenuControls_TryAdvanceState, "auto");
