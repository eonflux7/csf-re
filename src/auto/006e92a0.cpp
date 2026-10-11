#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixMultiply, 0x00624630, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(double, g_RadiansToDegrees, 0x00861DF8);
CSF_GLOBAL(uint32_t*, g_QuaternionMatrixA, 0x008BBABC);
CSF_GLOBAL(uint32_t*, g_QuaternionMatrixB, 0x008BBAC0);
CSF_GLOBAL(uint32_t*, g_ComposedMatrix, 0x008BBAC4);

static uint32_t* __fastcall C_00861e98_ComposeTransformsAndExtractPose(
    C_00861e98* self, void* /*edx*/,
    const void* firstTransform, const void* secondTransform,
    const void* thirdTransform, uint32_t* localPosition,
    float* headingDegrees, uint32_t* worldPosition, uint32_t* worldDirection)
{
    uint32_t* firstMatrix;
    uint32_t* secondMatrix;

    {
        const float x = CSF_AT(float, firstTransform, 0x08);
        const float y = CSF_AT(float, firstTransform, 0x0C);
        const float z = CSF_AT(float, firstTransform, 0x10);
        const float w = CSF_AT(float, firstTransform, 0x14);
        firstMatrix = g_QuaternionMatrixA;

        const float xx = x * x;
        const float yy = y * y;
        const float zz = z * z;
        const float yz = z * y;
        const float xz = z * x;
        const float xy = y * x;
        const float wx = w * x;
        const float wy = w * y;
        const float wz = w * z;

        CSF_AT(float, firstMatrix, 0x00) = g_TransitionOne - (zz + yy + zz + yy);
        CSF_AT(float, firstMatrix, 0x04) = xy + wz + xy + wz;
        CSF_AT(float, firstMatrix, 0x08) = xz - wy + xz - wy;
        CSF_AT(float, firstMatrix, 0x10) = xy - wz + xy - wz;
        CSF_AT(float, firstMatrix, 0x14) = g_TransitionOne - (zz + xx + zz + xx);
        CSF_AT(float, firstMatrix, 0x18) = wx + yz + wx + yz;
        CSF_AT(float, firstMatrix, 0x20) = wy + xz + wy + xz;
        CSF_AT(float, firstMatrix, 0x24) = yz - wx + yz - wx;
        CSF_AT(uint32_t, firstMatrix, 0x30) = 0;
        CSF_AT(uint32_t, firstMatrix, 0x34) = 0;
        CSF_AT(uint32_t, firstMatrix, 0x38) = 0;
        CSF_AT(uint32_t, firstMatrix, 0x0C) = 3;
        CSF_AT(float, firstMatrix, 0x28) = g_TransitionOne - (yy + xx + yy + xx);
        CSF_AT(uint32_t, firstMatrix, 0x30) = CSF_AT(uint32_t, firstTransform, 0x18);
        CSF_AT(uint32_t, firstMatrix, 0x34) = CSF_AT(uint32_t, firstTransform, 0x1C);
        CSF_AT(uint32_t, firstMatrix, 0x38) = CSF_AT(uint32_t, firstTransform, 0x20);
    }

    {
        const float x = CSF_AT(float, secondTransform, 0x08);
        const float y = CSF_AT(float, secondTransform, 0x0C);
        const float z = CSF_AT(float, secondTransform, 0x10);
        const float w = CSF_AT(float, secondTransform, 0x14);
        secondMatrix = g_QuaternionMatrixB;

        const float xx = x * x;
        const float yy = y * y;
        const float zz = z * z;
        const float yz = z * y;
        const float xz = z * x;
        const float xy = y * x;
        const float wx = w * x;
        const float wy = w * y;
        const float wz = w * z;

        CSF_AT(float, secondMatrix, 0x00) = g_TransitionOne - (zz + yy + zz + yy);
        CSF_AT(float, secondMatrix, 0x04) = xy + wz + xy + wz;
        CSF_AT(float, secondMatrix, 0x08) = xz - wy + xz - wy;
        CSF_AT(float, secondMatrix, 0x10) = xy - wz + xy - wz;
        CSF_AT(float, secondMatrix, 0x14) = g_TransitionOne - (zz + xx + zz + xx);
        CSF_AT(uint32_t, secondMatrix, 0x30) = 0;
        CSF_AT(uint32_t, secondMatrix, 0x34) = 0;
        CSF_AT(uint32_t, secondMatrix, 0x38) = 0;
        CSF_AT(uint32_t, secondMatrix, 0x0C) = 3;

        uint32_t* composedMatrix = g_ComposedMatrix;
        CSF_AT(float, secondMatrix, 0x18) = wx + yz + wx + yz;
        CSF_AT(float, secondMatrix, 0x20) = wy + xz + wy + xz;
        CSF_AT(float, secondMatrix, 0x24) = yz - wx + yz - wx;
        CSF_AT(float, secondMatrix, 0x28) = g_TransitionOne - (yy + xx + yy + xx);
        CSF_AT(uint32_t, secondMatrix, 0x30) = CSF_AT(uint32_t, secondTransform, 0x18);
        CSF_AT(uint32_t, secondMatrix, 0x34) = CSF_AT(uint32_t, secondTransform, 0x1C);
        CSF_AT(uint32_t, secondMatrix, 0x38) = CSF_AT(uint32_t, secondTransform, 0x20);

        RwMatrixMultiply(composedMatrix, firstMatrix, secondMatrix);
    }

    {
        const float x = CSF_AT(float, thirdTransform, 0x08);
        const float y = CSF_AT(float, thirdTransform, 0x0C);
        const float z = CSF_AT(float, thirdTransform, 0x10);
        const float w = CSF_AT(float, thirdTransform, 0x14);
        firstMatrix = g_QuaternionMatrixA;
        secondMatrix = g_QuaternionMatrixB;

        const float xx = x * x;
        const float yy = y * y;
        const float zz = z * z;
        const float yz = z * y;
        const float xz = z * x;
        const float xy = y * x;
        const float wx = w * x;
        const float wy = w * y;
        const float wz = w * z;

        CSF_AT(float, firstMatrix, 0x00) = g_TransitionOne - (zz + yy + zz + yy);
        CSF_AT(uint32_t, firstMatrix, 0x30) = 0;
        CSF_AT(uint32_t, firstMatrix, 0x34) = 0;
        CSF_AT(uint32_t, firstMatrix, 0x38) = 0;
        CSF_AT(uint32_t, firstMatrix, 0x0C) = 3;
        CSF_AT(float, firstMatrix, 0x04) = xy + wz + xy + wz;
        CSF_AT(float, firstMatrix, 0x08) = xz - wy + xz - wy;
        CSF_AT(float, firstMatrix, 0x10) = xy - wz + xy - wz;
        CSF_AT(float, firstMatrix, 0x14) = g_TransitionOne - (zz + xx + zz + xx);
        CSF_AT(float, firstMatrix, 0x18) = wx + yz + wx + yz;
        CSF_AT(float, firstMatrix, 0x20) = wy + xz + wy + xz;
        CSF_AT(float, firstMatrix, 0x24) = yz - wx + yz - wx;
        CSF_AT(float, firstMatrix, 0x28) = g_TransitionOne - (yy + xx + yy + xx);
        CSF_AT(uint32_t, firstMatrix, 0x30) = CSF_AT(uint32_t, thirdTransform, 0x18);
        CSF_AT(uint32_t, firstMatrix, 0x34) = CSF_AT(uint32_t, thirdTransform, 0x1C);
        CSF_AT(uint32_t, firstMatrix, 0x38) = CSF_AT(uint32_t, thirdTransform, 0x20);

        RwMatrixMultiply(secondMatrix, g_ComposedMatrix, firstMatrix);
    }

    if (localPosition)
    {
        localPosition[0] = CSF_AT(uint32_t, secondMatrix, 0x30);
        localPosition[1] = CSF_AT(uint32_t, secondMatrix, 0x34);
        localPosition[2] = CSF_AT(uint32_t, secondMatrix, 0x38);
    }

    if (headingDegrees)
    {
        alignas(8) uint8_t directionBytes[0x20];
        S_00526d40& direction = *reinterpret_cast<S_00526d40*>(directionBytes);
        CSF_AT(uint32_t, directionBytes, 0x00) = CSF_AT(uint32_t, secondMatrix, 0x20);
        CSF_AT(float, directionBytes, 0x04) = CSF_AT(float, secondMatrix, 0x24);
        CSF_AT(float, directionBytes, 0x08) = CSF_AT(float, secondMatrix, 0x28);
        *headingDegrees = static_cast<float>(
            S_00526d40_GetVectorAngleRadians(&direction, nullptr) * g_RadiansToDegrees);
    }

    uint32_t* worldMatrix = g_ComposedMatrix;
    RwMatrixMultiply(g_ComposedMatrix, secondMatrix,
                     reinterpret_cast<uint32_t*>(self->field_0048));

    if (worldPosition)
    {
        worldPosition[0] = CSF_AT(uint32_t, worldMatrix, 0x30);
        worldPosition[1] = CSF_AT(uint32_t, worldMatrix, 0x34);
        worldPosition[2] = CSF_AT(uint32_t, worldMatrix, 0x38);
    }

    if (worldDirection)
    {
        worldDirection[0] = CSF_AT(uint32_t, worldMatrix, 0x20);
        worldDirection[1] = CSF_AT(uint32_t, worldMatrix, 0x24);
        worldDirection[2] = CSF_AT(uint32_t, worldMatrix, 0x28);
    }

    return worldDirection;
}

HOOK(0x006E92A0, C_00861e98_ComposeTransformsAndExtractPose, "auto");
