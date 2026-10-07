#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Constant float loaded by the original (fld dword ptr [0x82D4C4]).
CSF_GLOBAL(float, g_flt_82D4C4, 0x82D4C4);

// Returns the shared constant value (-1.0f), promoted to double by the FPU load.
static double __cdecl C_008619e0_GetConstantValue(void)
{
    return static_cast<double>(g_flt_82D4C4);
}

HOOK(0x6E03C0, C_008619e0_GetConstantValue, "auto");
