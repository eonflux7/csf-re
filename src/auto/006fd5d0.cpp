#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpLightSetRadius, 0x0059CBD0, (int a1, int a2));
CSF_FUNC(void, __fastcall, C_00860e68_AddLightIfNotCurrent, 0x006D2000, (C_00860e68* self, void* /*edx*/, C_0085e7b8* a2));
CSF_FUNC(uint32_t*, __fastcall, Light_GetWorldMatrixPointer, 0x006FD330, (void* self, void* /*edx*/));

// Sets the radius of the RpLight owned by this object, clears the flag at +0x34,
// refreshes the world matrix pointer, and re-registers the light with its owner
// if it is not already the current light.
static void __fastcall CLightObject_SetRadius(CLightObject* self, void* /*edx*/, int radius)
{
    uint32_t rpLight = CSF_AT(uint32_t, self, 0x0C);
    if (rpLight != 0)
    {
        RpLightSetRadius(static_cast<int>(rpLight), radius);
        CSF_AT(uint8_t, self, 0x34) = 0;
        Light_GetWorldMatrixPointer(self, nullptr);
        C_00860e68_AddLightIfNotCurrent(
            reinterpret_cast<C_00860e68*>(CSF_AT(uint32_t, self, 0x08)),
            nullptr,
            reinterpret_cast<C_0085e7b8*>(self));
    }
}

HOOK(0x006FD5D0, CLightObject_SetRadius, "auto");
