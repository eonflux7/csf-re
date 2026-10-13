#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: RpLightSetConeAngle (__cdecl, returns int)
CSF_FUNC(int, __cdecl, RpLightSetConeAngle, 0x0059CE80, (int a1, float a2));

// Sets the spot-light cone angle on the RpLight stored at +0x0C of this.
static int __fastcall CLightObject_SetConeAngle(void* self, void* /*edx*/, float coneAngle)
{
    int lightPointer = static_cast<int>(CSF_AT(uint32_t, self, 0x0C));
    return RpLightSetConeAngle(lightPointer, coneAngle);
}

HOOK(0x6FD190, CLightObject_SetConeAngle, "auto");
