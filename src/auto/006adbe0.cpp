#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reads the rotation factor constant (180/pi) stored in the game's data section.
CSF_GLOBAL(const float, g_RadiansToDegreesFactor, 0x0086008C);

// RW engine matrix rotate: (matrix, axis-or-vector pointer, angle in degrees, axis id).
CSF_FUNC(uint32_t*, __cdecl, RwMatrixRotate, 0x006248B0,
         (uint32_t* a1, float* a2, float a3, int a4));

// Rotates the matrix held in *self around the Z axis (axis id 2).
// The angle arrives in radians and is converted to degrees before the RW call.
static int __fastcall GfxMatrix_RotateZAxisRadians(int* self, void* /*edx*/,
                                                   int a2, float a3)
{
    float degrees = a3 * g_RadiansToDegreesFactor;
    return reinterpret_cast<int>(RwMatrixRotate(
        reinterpret_cast<uint32_t*>(*self),
        reinterpret_cast<float*>(a2),
        degrees,
        2));
}

HOOK(0x6ADBE0, GfxMatrix_RotateZAxisRadians, "auto");
