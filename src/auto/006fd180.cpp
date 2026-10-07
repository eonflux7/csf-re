#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// double __cdecl RpLightGetConeAngle(int light)
CSF_FUNC(double, __cdecl, RpLightGetConeAngle, 0x59CC50, (int light));

// void __thiscall sub_6FD180(CLightObject* this)
// Reads the light pointer stored at this+0x0C and queries its cone angle.
static void __fastcall CLightObject_GetLightConeAngle(CLightObject* self, void* /*edx*/)
{
    RpLightGetConeAngle(static_cast<int>(self->m_pLight));
}

HOOK(0x6FD180, CLightObject_GetLightConeAngle, "auto");
