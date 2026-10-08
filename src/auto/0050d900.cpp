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

// Callee: returns pointer to the global value block (cdecl, no args).
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x753A30, ());

CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);

// Returns a transition scale for the menu object. Falls back to the global
// default when the flag byte at +0xD0 is set; otherwise picks one of three
// per-state floats from the global value block based on the state at +0x84.
// Unmatched states fall through to the default value.
static float __fastcall MenuSystem_GetTransitionScale(void* self, void* /*edx*/)
{
    // The original loads the default constant before any branching.
    const float defaultValue = g_TransitionOne;

    if (CSF_AT(uint8_t, self, 0xD0) == 0)
    {
        const int32_t stateCode = CSF_AT(int32_t, self, 0x84);
        if (stateCode == 0)
        {
            return CSF_AT(float, GetGlobalValue(), 0xD0);
        }
        if (stateCode == 2)
        {
            return CSF_AT(float, GetGlobalValue(), 0xD4);
        }
        if (stateCode == 3)
        {
            return CSF_AT(float, GetGlobalValue(), 0xD8);
        }
    }
    return defaultValue;
}

HOOK(0x50D900, MenuSystem_GetTransitionScale, "auto");
