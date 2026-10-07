#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the 32-bit float stored at offset 0x3D4 of the object.
// The original loads it with `fld dword ptr [ecx+0x3d4]` and returns it in ST(0).
static double __fastcall GetFloat3D4(void* self, void* /*edx*/)
{
    return CSF_AT(float, self, 0x3D4);
}

HOOK(0x4954E0, GetFloat3D4, "auto");
