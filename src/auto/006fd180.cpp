#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// double __cdecl RpLightGetConeAngle(int light)
CSF_FUNC(double, __cdecl, RpLightGetConeAngle, 0x59CC50, (int light));

// void __thiscall sub_6FD180(C_00862f70* this)
// Reads the light pointer stored at this+0x0C and queries its cone angle.
static void __fastcall C_00862f70_GetLightConeAngle(C_00862f70* self, void* /*edx*/)
{
    RpLightGetConeAngle(static_cast<int>(self->field_000c));
}

HOOK(0x6FD180, C_00862f70_GetLightConeAngle, "auto");
