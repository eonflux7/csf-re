#include <cstdint>
#include "globals.h"
#include "hook.h"

// The original loads a 32-bit float constant with `fld dword ptr [0x82E0F0]`
// and leaves it on the x87 stack as the returned double (sub_41DD80).
CSF_GLOBAL(float, g_Const20, 0x0082E0F0);

static double __cdecl C_0082e4c0_GetTwenty()
{
    return static_cast<double>(g_Const20);
}

HOOK(0x41DD80, C_0082e4c0_GetTwenty, "auto");
