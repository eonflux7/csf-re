#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(double, __cdecl, CalculateArcCosine, 0x0040C980, (float a1));

static double __fastcall GfxHAnim_GetQuaternionAngle(void* self, void* /*edx*/)
{
    const double angle = CalculateArcCosine(CSF_AT(float, self, 0x0C));
    return angle + angle;
}

HOOK(0x006AEA20, GfxHAnim_GetQuaternionAngle, "auto");
