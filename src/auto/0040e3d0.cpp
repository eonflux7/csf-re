#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fDegToRad, 0x0082DD5C);

// Stores the global angle value (global block +0x1D74, in degrees) converted to radians
// into the float at this+0x54. The sign of the stored value follows the sign of the argument;
// a zero argument leaves the field untouched.
static void __fastcall SetAngleFromGlobalDegrees(float* self, void* /*edx*/, float angleSign)
{
    if (angleSign > g_fZero)
    {
        self[21] = CSF_AT(float, reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0()), 0x1D74) * g_fDegToRad;
    }
    else if (angleSign < g_fZero)
    {
        self[21] = -(CSF_AT(float, reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0()), 0x1D74) * g_fDegToRad);
    }
}

HOOK(0x0040E3D0, SetAngleFromGlobalDegrees, "auto");
