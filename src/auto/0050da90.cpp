#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original: 0x0050DA90, __thiscall, returns a float (via st0) selected by the
// current index stored at this+0x84. Index 0..3 picks one of four float fields
// from the global value block; any other index yields g_FloatMax.

CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x753A30, ());
CSF_GLOBAL(float, g_FloatMax, 0x831090);

static double __fastcall MenuSystem_GetIndexedGlobalFloat(void* self, void* /*edx*/)
{
    const uint32_t selectorIndex = CSF_AT(uint32_t, self, 0x84);

    switch (selectorIndex)
    {
    case 0:
        return static_cast<double>(CSF_AT(float, GetGlobalValue_753A30(), 0x10C));
    case 1:
        return static_cast<double>(CSF_AT(float, GetGlobalValue_753A30(), 0x110));
    case 2:
        return static_cast<double>(CSF_AT(float, GetGlobalValue_753A30(), 0x114));
    case 3:
        return static_cast<double>(CSF_AT(float, GetGlobalValue_753A30(), 0x118));
    default:
        return static_cast<double>(g_FloatMax);
    }
}

HOOK(0x0050DA90, MenuSystem_GetIndexedGlobalFloat, "auto");
