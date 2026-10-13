#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);

// Accumulates a2 into the panel value at +0x174. The sum is kept only while it stays
// below g_TransitionOne; otherwise the value resets to 0.0.
static void __fastcall HudSpritesLayout_SetPanelValue(float* self, void* /*edx*/, float a2)
{
    float accumulated = a2 + CSF_AT(float, self, 0x174);
    CSF_AT(float, self, 0x174) = accumulated;

    if (accumulated < g_TransitionOne) {
        CSF_AT(float, self, 0x174) = accumulated;
    } else {
        CSF_AT(float, self, 0x174) = 0.0f;
    }
}

HOOK(0x436C50, HudSpritesLayout_SetPanelValue, "auto");
