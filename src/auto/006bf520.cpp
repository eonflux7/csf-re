#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyVec3AndUpdate, 0x006AD950, (uint32_t* self, void* , uint32_t* src));
CSF_FUNC(int, __fastcall, GfxMatrix_SetTranslationAndUpdate, 0x006AD980, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, GfxMatrix_SetScaleAndUpdate, 0x006AD9B0, (void* self, void* , const uint32_t* src));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetEulerAngles, 0x006AE3A0, (float** self, void* , float* angles));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_RadiansToDegrees, 0x0086061C);

static char __fastcall C_00860700_SetFrameDirection(
    C_00860700* self, void* /*edx*/, S_00526d40* directionSource)
{
    alignas(8) uint8_t anglesBytes[0x18];
    alignas(8) uint8_t upBytes[0xC];
    alignas(8) uint8_t rightBytes[0xC];
    alignas(8) uint8_t directionBytes[0xC];
    alignas(8) uint8_t matrixBytes[0x8];

    auto* direction = reinterpret_cast<float*>(directionBytes);
    auto* right = reinterpret_cast<float*>(rightBytes);
    auto* up = reinterpret_cast<float*>(upBytes);

    S_00526d40_ConvertDirection(
        directionSource, nullptr, reinterpret_cast<uint32_t*>(directionBytes));

    up[0] = 0.0f;
    up[1] = 1.0f;
    up[2] = 0.0f;

    const float zeroProduct = direction[1] * g_fZero;
    right[0] = direction[2] - zeroProduct;
    right[1] = direction[0] * g_fZero - direction[2] * g_fZero;
    right[2] = zeroProduct - direction[0];

    // Compiler intrinsic: the original uses fsqrt, not a CRT call.
    const float length = __builtin_sqrtf(
        right[2] * right[2] +
        right[1] * right[1] +
        right[0] * right[0]);

    if (g_fZero != length)
    {
        NormalizeVector3(reinterpret_cast<C_00833b88*>(rightBytes), nullptr);
        up[0] = right[2] * direction[1] - right[1] * direction[2];
        up[1] = direction[2] * right[0] - right[2] * direction[0];
        up[2] = right[1] * direction[0] - direction[1] * right[0];
    }

    CSF_AT(uint32_t, matrixBytes, 0x0) = self->field_000c + 16;
    CSF_AT(char, matrixBytes, 0x4) = 0;

    GfxMatrix_CopyVec3AndUpdate(
        reinterpret_cast<uint32_t*>(matrixBytes), nullptr,
        reinterpret_cast<uint32_t*>(rightBytes));
    GfxMatrix_SetScaleAndUpdate(
        matrixBytes, nullptr, reinterpret_cast<const uint32_t*>(directionBytes));
    GfxMatrix_SetTranslationAndUpdate(
        matrixBytes, nullptr, reinterpret_cast<const uint32_t*>(upBytes));

    float* angles = GfxMatrix_GetEulerAngles(
        reinterpret_cast<float**>(matrixBytes), nullptr,
        reinterpret_cast<float*>(anglesBytes));

    const float pitch = angles[0] * g_RadiansToDegrees;
    const float yaw = angles[1] * g_RadiansToDegrees;
    const float roll = angles[2] * g_RadiansToDegrees;
    self->field_0014 = pitch;
    CSF_AT(float, self, 0x18) = yaw;
    const uint32_t frame = self->field_000c;
    CSF_AT(float, self, 0x1C) = roll;

    RwFrameUpdateObjects(static_cast<int>(frame));

    const char ownsMatrix = CSF_AT(char, matrixBytes, 0x4);
    if (ownsMatrix)
    {
        return static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(
                CSF_AT(uint32_t, matrixBytes, 0x0))));
    }
    return ownsMatrix;
}

HOOK(0x006BF520, C_00860700_SetFrameDirection, "auto");
