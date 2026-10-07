#include <cstdint>

#include "globals.h"
#include "hook.h"

// Float constant read by the function (value: 50.0f).
CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);

// Trivial accessor: loads the 32-bit float constant stored at 0x0082E030
// (disassembly: `fld dword ptr [0x82e030]; ret`). The original returns the
// value on the x87 stack, so the natural return type is float.
static float __cdecl GetReturnedConstant()
{
    return g_fReturnedConstant_0082E030;
}

HOOK(0x00412070, GetReturnedConstant, "auto");
