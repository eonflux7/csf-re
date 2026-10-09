#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Data bound by address (original .rdata/.data)
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_FloatMax, 0x00831090);

// Returns the instance's fade time, or g_FloatMax when the fade time is exactly zero.
// Original: __thiscall, ret 0, returns a double on the x87 stack.
static double __fastcall CSoundInstance_GetFadeTimeOrMax(CSoundInstance* self, void* /*edx*/)
{
    if (self->m_fadeTime == g_fZero)
        return g_FloatMax;
    return self->m_fadeTime;
}

HOOK(0x526860, CSoundInstance_GetFadeTimeOrMax, "auto");
