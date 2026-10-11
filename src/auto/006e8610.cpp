#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixMultiply, 0x00624630, (uint32_t* a1, uint32_t* a2, uint32_t* a3));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(uint32_t*, g_QuaternionMatrix, 0x008BBABC);
CSF_GLOBAL(uint32_t*, g_ComposedMatrix, 0x008BBAC4);

static int __fastcall C_00861e98_BuildTransformMatrix(
    C_00861e98* self, void* /*edx*/, int transformAddress)
{
    const void* transform = reinterpret_cast<const void*>(transformAddress);
    const float x = CSF_AT(float, transform, 0x08);
    const float y = CSF_AT(float, transform, 0x0C);
    const float z = CSF_AT(float, transform, 0x10);
    const float w = CSF_AT(float, transform, 0x14);
    uint32_t* matrix = g_QuaternionMatrix;

    const float xx = x * x;
    const float yy = y * y;
    const float zz = z * z;
    const float zy = z * y;
    const float zx = z * x;
    const float yx = y * x;
    const float wx = w * x;
    const float wy = w * y;
    const float wz = w * z;

    CSF_AT(float, matrix, 0x00) = g_TransitionOne - (zz + yy + zz + yy);
    CSF_AT(float, matrix, 0x04) = yx + wz + yx + wz;
    CSF_AT(float, matrix, 0x08) = zx - wy + zx - wy;
    CSF_AT(float, matrix, 0x10) = yx - wz + yx - wz;
    CSF_AT(float, matrix, 0x14) = g_TransitionOne - (zz + xx + zz + xx);
    CSF_AT(float, matrix, 0x18) = wx + zy + wx + zy;
    CSF_AT(float, matrix, 0x20) = wy + zx + wy + zx;
    CSF_AT(float, matrix, 0x24) = zy - wx + zy - wx;

    CSF_AT(uint32_t, matrix, 0x30) = 0;
    CSF_AT(uint32_t, matrix, 0x34) = 0;
    CSF_AT(uint32_t, matrix, 0x38) = 0;
    CSF_AT(uint32_t, matrix, 0x0C) = 3;
    CSF_AT(float, matrix, 0x28) = g_TransitionOne - (yy + xx + yy + xx);

    CSF_AT(uint32_t, matrix, 0x30) = CSF_AT(uint32_t, transform, 0x18);
    CSF_AT(uint32_t, matrix, 0x34) = CSF_AT(uint32_t, transform, 0x1C);
    CSF_AT(uint32_t, matrix, 0x38) = CSF_AT(uint32_t, transform, 0x20);

    uint32_t* baseMatrix = CSF_AT(uint32_t*, self, 0x48);
    uint32_t* result = g_ComposedMatrix;
    RwMatrixMultiply(result, matrix, baseMatrix);
    return reinterpret_cast<int>(result);
}

HOOK(0x006E8610, C_00861e98_BuildTransformMatrix, "auto");
