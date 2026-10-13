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

CSF_FUNC(int, __cdecl, RtAnimRegisterInterpolationScheme, 0x0061ED00, (uint32_t* a1));

// Builds the interpolation-scheme descriptor table and registers it with the animation runtime.
// Returns non-zero (TRUE) when the runtime accepts the scheme.
static int __cdecl AnimRegisterInterpolationSchemeTable()
{
    uint32_t schemeTable[12];

    schemeTable[0] = 273;                                                     // scheme id 0x111
    schemeTable[1] = 36;                                                      // descriptor size 0x24
    schemeTable[2] = 16;                                                      // count
    schemeTable[3] = (uint32_t)(uintptr_t)CSF_ADDR_FN(0x006C25E0);
    schemeTable[4] = (uint32_t)(uintptr_t)CSF_ADDR_FN(0x006C2EB0);
    schemeTable[5] = (uint32_t)(uintptr_t)CSF_ADDR_FN(0x006C26E0);
    schemeTable[6] = (uint32_t)(uintptr_t)CSF_ADDR_FN(0x006C2C20);
    schemeTable[7] = (uint32_t)(uintptr_t)CSF_ADDR_FN(0x006C2AD0);
    schemeTable[8] = (uint32_t)(uintptr_t)CSF_ADDR_FN(0x006C2AB0);           // allocator slot (Hex-Rays: operator new)
    schemeTable[9] = (uint32_t)(uintptr_t)CSF_ADDR_FN(0x004206B0);
    schemeTable[10] = (uint32_t)(uintptr_t)CSF_ADDR_FN(0x006C2AC0);          // GetScaledCountBy16
    schemeTable[11] = 0;

    return RtAnimRegisterInterpolationScheme(schemeTable) != 0;
}

HOOK(0x006C34F0, AnimRegisterInterpolationSchemeTable, "auto");
