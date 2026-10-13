#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static double __fastcall NetCallbacks_GetGameModeFloatParameter(
    float* self, void* /*edx*/, int parameterId)
{
    switch (parameterId)
    {
    case 13: return CSF_AT(float, self, 0x08);
    case 14: return CSF_AT(float, self, 0x38);
    case 15: return CSF_AT(float, self, 0x30);
    case 16: return CSF_AT(float, self, 0x28);
    case 17: return CSF_AT(float, self, 0x20);
    case 18: return CSF_AT(float, self, 0x18);
    case 19: return CSF_AT(float, self, 0x10);
    case 22: return CSF_AT(float, self, 0x04);
    case 23: return CSF_AT(float, self, 0x34);
    case 24: return CSF_AT(float, self, 0x2C);
    case 25: return CSF_AT(float, self, 0x24);
    case 26: return CSF_AT(float, self, 0x1C);
    case 27: return CSF_AT(float, self, 0x14);
    case 28: return CSF_AT(float, self, 0x0C);
    default: return g_fZero;
    }
}

HOOK(0x007442A0, NetCallbacks_GetGameModeFloatParameter, "auto");
