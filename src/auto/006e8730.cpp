#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixMultiply, 0x00624630, (uint32_t* a1, uint32_t* a2, uint32_t* a3));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(uint32_t*, g_FirstTransformMatrix, 0x008BBABC);
CSF_GLOBAL(uint32_t*, g_SecondTransformMatrix, 0x008BBAC0);
CSF_GLOBAL(uint32_t*, g_ComposedTransformMatrix, 0x008BBAC4);

static int __fastcall C_00861e98_ComposeTransforms(
    C_00861e98* self, void* /*edx*/,
    const void* firstTransform, const void* secondTransform)
{
    const float x = CSF_AT(float, firstTransform, 0x08);
    const float y = CSF_AT(float, firstTransform, 0x0C);
    const float z = CSF_AT(float, firstTransform, 0x10);
    const float w = CSF_AT(float, firstTransform, 0x14);
    uint32_t* firstMatrix = g_FirstTransformMatrix;

    const float xx = x * x;
    const float yy = y * y;
    const float zz = z * z;
    const float zy = z * y;
    const float zx = z * x;
    const float yx = y * x;
    const float wx = w * x;
    const float wy = w * y;
    const float wz = w * z;

    CSF_AT(float, firstMatrix, 0x00) = g_TransitionOne - (zz + yy + zz + yy);
    CSF_AT(float, firstMatrix, 0x04) = yx + wz + yx + wz;
    CSF_AT(float, firstMatrix, 0x08) = zx - wy + zx - wy;
    CSF_AT(float, firstMatrix, 0x10) = yx - wz + yx - wz;
    CSF_AT(float, firstMatrix, 0x14) = g_TransitionOne - (zz + xx + zz + xx);
    CSF_AT(float, firstMatrix, 0x18) = wx + zy + wx + zy;
    CSF_AT(float, firstMatrix, 0x20) = wy + zx + wy + zx;
    CSF_AT(float, firstMatrix, 0x24) = zy - wx + zy - wx;
    CSF_AT(uint32_t, firstMatrix, 0x0C) = 3;
    CSF_AT(uint32_t, firstMatrix, 0x30) = 0;
    CSF_AT(uint32_t, firstMatrix, 0x34) = 0;
    CSF_AT(uint32_t, firstMatrix, 0x38) = 0;
    CSF_AT(float, firstMatrix, 0x28) = g_TransitionOne - (yy + xx + yy + xx);
    CSF_AT(uint32_t, firstMatrix, 0x30) = CSF_AT(uint32_t, firstTransform, 0x18);
    CSF_AT(uint32_t, firstMatrix, 0x34) = CSF_AT(uint32_t, firstTransform, 0x1C);
    CSF_AT(uint32_t, firstMatrix, 0x38) = CSF_AT(uint32_t, firstTransform, 0x20);

    const float secondX = CSF_AT(float, secondTransform, 0x08);
    const float secondY = CSF_AT(float, secondTransform, 0x0C);
    const float secondZ = CSF_AT(float, secondTransform, 0x10);
    const float secondW = CSF_AT(float, secondTransform, 0x14);
    uint32_t* secondMatrix = g_SecondTransformMatrix;

    const float secondXX = secondX * secondX;
    const float secondYY = secondY * secondY;
    const float secondZZ = secondZ * secondZ;
    const float secondZY = secondZ * secondY;
    const float secondZX = secondZ * secondX;
    const float secondYX = secondY * secondX;
    const float secondWX = secondW * secondX;
    const float secondWY = secondW * secondY;
    const float secondWZ = secondW * secondZ;

    CSF_AT(float, secondMatrix, 0x00) =
        g_TransitionOne - (secondZZ + secondYY + secondZZ + secondYY);
    CSF_AT(float, secondMatrix, 0x04) = secondYX + secondWZ + secondYX + secondWZ;
    CSF_AT(float, secondMatrix, 0x08) = secondZX - secondWY + secondZX - secondWY;
    CSF_AT(float, secondMatrix, 0x10) = secondYX - secondWZ + secondYX - secondWZ;
    CSF_AT(float, secondMatrix, 0x14) =
        g_TransitionOne - (secondZZ + secondXX + secondZZ + secondXX);
    CSF_AT(uint32_t, secondMatrix, 0x30) = 0;
    CSF_AT(uint32_t, secondMatrix, 0x34) = 0;
    CSF_AT(uint32_t, secondMatrix, 0x38) = 0;
    CSF_AT(uint32_t, secondMatrix, 0x0C) = 3;
    CSF_AT(float, secondMatrix, 0x18) = secondWX + secondZY + secondWX + secondZY;

    uint32_t* composedMatrix = g_ComposedTransformMatrix;
    CSF_AT(float, secondMatrix, 0x20) = secondWY + secondZX + secondWY + secondZX;
    CSF_AT(float, secondMatrix, 0x24) = secondZY - secondWX + secondZY - secondWX;
    CSF_AT(float, secondMatrix, 0x28) =
        g_TransitionOne - (secondYY + secondXX + secondYY + secondXX);
    CSF_AT(uint32_t, secondMatrix, 0x30) = CSF_AT(uint32_t, secondTransform, 0x18);
    CSF_AT(uint32_t, secondMatrix, 0x34) = CSF_AT(uint32_t, secondTransform, 0x1C);
    CSF_AT(uint32_t, secondMatrix, 0x38) = CSF_AT(uint32_t, secondTransform, 0x20);

    RwMatrixMultiply(composedMatrix, firstMatrix, secondMatrix);
    RwMatrixMultiply(g_SecondTransformMatrix, g_ComposedTransformMatrix,
                     CSF_AT(uint32_t*, self, 0x48));
    return static_cast<int>(reinterpret_cast<std::intptr_t>(g_SecondTransformMatrix));
}

HOOK(0x006E8730, C_00861e98_ComposeTransforms, "auto");
