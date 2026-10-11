#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns 1 when the float stored at +0x08 is strictly below zero, 0 otherwise
// (including NaN, which the original's fcomp/parity test also rejects).
static int __fastcall AiAttack_IsStoredValueNegative(float* self, void* /*edx*/)
{
    float storedValue = CSF_AT(float, self, 0x08);
    return storedValue < g_fZero ? 1 : 0;
}

HOOK(0x0047AEB0, AiAttack_IsStoredValueNegative, "auto");
