#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpLightSetColor, 0x0059CBF0, (int a1, uint32_t* a2));

// Recomputes the light colour from the object's scale/offset terms and pushes it to the RenderWare light.
// Returns the light's pending-update flag (byte at +0x81) and clears it; no update happens when it is zero.
static char __fastcall C_00861988_ApplyParameters(C_00861988* self, void* /*edx*/)
{
    char pendingFlag = static_cast<char>(CSF_AT(uint8_t, self, 0x81));
    if (pendingFlag == 0)
        return pendingFlag;

    bool useAlternateScale = CSF_AT(uint8_t, self, 0x80) != 0;
    CSF_AT(uint8_t, self, 0x81) = 0;

    float* scale = useAlternateScale ? &CSF_AT(float, self, 0x70)
                                     : &CSF_AT(float, self, 0x40);

    float colorRgba[4];
    colorRgba[0] = CSF_AT(float, self, 0x50) * scale[0] + CSF_AT(float, self, 0x60);
    colorRgba[1] = CSF_AT(float, self, 0x54) * scale[1] + CSF_AT(float, self, 0x64);
    colorRgba[2] = CSF_AT(float, self, 0x58) * scale[2] + CSF_AT(float, self, 0x68);
    colorRgba[3] = CSF_AT(float, self, 0x5C) * scale[3] + CSF_AT(float, self, 0x6C);

    int lightHandle = static_cast<int>(CSF_AT(uint32_t, self, 0x0C));
    return static_cast<char>(RpLightSetColor(lightHandle, reinterpret_cast<uint32_t*>(colorRgba)));
}

HOOK(0x006FD0E0, C_00861988_ApplyParameters, "auto");
