#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Float constant living in the read-only data section (value is 200.0f).
CSF_GLOBAL(float, g_Const200_0f, 0x82DDEC);

// Reads the global float constant and returns it (converted to double by the
// x87 FPU, exactly like the original `fld dword ptr [...]`).
static double __cdecl GetConst200()
{
    return g_Const200_0f;
}

HOOK(0x40FE20, GetConst200, "auto");
