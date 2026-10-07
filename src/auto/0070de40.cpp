#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global float value read at call time (fld dword ptr [0x82D4CC]).
CSF_GLOBAL(float, g_Value_0082D4CC, 0x82D4CC);

// Loads the global float and returns it widened to double (fld + ret 4).
static double __stdcall GetGlobalFloatValue(int /*unused*/)
{
    return static_cast<double>(g_Value_0082D4CC);
}

HOOK(0x70DE40, GetGlobalFloatValue, "auto");
