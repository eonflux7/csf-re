#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Audio_ApplyEffectLevel, 0x00712AB0, (float a1));
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Clamps a factor into [0, 1] (NaN passes through unchanged, matching the x87 compares),
// stores it in the object, then forwards it to the audio effect applier.
static int __fastcall C_00863ba8_SetClampedEffectLevel(C_00863ba8* self, void* /*edx*/, float a2)
{
    if (a2 > g_TransitionOne)
        a2 = g_TransitionOne;
    else if (a2 < g_fZero)
        a2 = g_fZero;

    self->field_002c = a2;
    return Audio_ApplyEffectLevel(a2);
}

HOOK(0x0070F420, C_00863ba8_SetClampedEffectLevel, "auto");
