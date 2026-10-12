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

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_fInv255, 0x00830A00);

// Updates the fade alpha of the screen-effect object. When the effect is in
// state 1, it resets parameter 2 and copies the scaled base value into
// parameter 5. The fade value is then decreased by a2 * 1000 and clamped to
// zero, and the result is written to parameter 5 as a 0..255 channel value.
static int __fastcall C_00862a80_UpdateFadeAlpha(C_00862a80* self, void* /*edx*/, float a2)
{
    if (self->vSlot20(2) == 1)
    {
        self->vSlot1C(2, nullptr);
        int baseValue = self->vSlot20(0);
        self->vSlot1C(5, reinterpret_cast<uint32_t*>(static_cast<intptr_t>(baseValue * 255)));
    }

    double fadeValue = static_cast<double>(self->vSlot20(5)) * g_fInv255 - a2 * g_flt_0082E25C;
    if (fadeValue < g_fZero)
        fadeValue = g_fZero;

    double scaledValue = fadeValue * g_ColorChannelScale;
    int64_t scaledInt = static_cast<int64_t>(scaledValue);
    int lowDword = static_cast<int>(scaledInt);
    return self->vSlot1C(5, reinterpret_cast<uint32_t*>(static_cast<intptr_t>(lowDword)));
}

HOOK(0x006F92E0, C_00862a80_UpdateFadeAlpha, "auto");
