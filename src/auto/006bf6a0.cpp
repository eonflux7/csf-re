#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(uint32_t*, __cdecl, RwMatrixTransform, 0x00624C70, (uint32_t* a1, uint32_t* a2, int a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetEulerAngles, 0x006AE3A0, (float** self, void* , float* angles));
CSF_FUNC(int, __fastcall, GfxMatrix_CreateQuaternionMatrix, 0x006AE6F0, (float* self, void* , int outputAddress));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_RadiansToDegrees, 0x0086061C);

static char __fastcall C_00860700_ApplyQuaternionRotation(
    C_00860700* self, void* /*edx*/, float* quaternion)
{
    alignas(8) uint8_t anglesBytes[0xC];
    alignas(8) uint8_t translationBytes[0x10];
    alignas(8) uint8_t quaternionMatrixBytes[0x8];
    alignas(8) uint8_t frameMatrixBytes[0x8];

    CSF_AT(uint32_t*, frameMatrixBytes, 0) =
        reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(self->field_000c) + 0x10);
    CSF_AT(uint8_t, frameMatrixBytes, 0x4) = 0;

    CSF_AT(uint32_t, translationBytes, 0) =
        CSF_AT(uint32_t, CSF_AT(uint32_t*, frameMatrixBytes, 0), 0x30);
    CSF_AT(uint32_t, translationBytes, 0x4) =
        CSF_AT(uint32_t, CSF_AT(uint32_t*, frameMatrixBytes, 0), 0x34);
    CSF_AT(uint32_t, translationBytes, 0x8) =
        CSF_AT(uint32_t, CSF_AT(uint32_t*, frameMatrixBytes, 0), 0x38);

    GfxMatrix_CopyTranslationAndUpdate(
        frameMatrixBytes, nullptr,
        reinterpret_cast<const uint32_t*>(&g_ZeroVectorX));

    const int quaternionMatrixAddress = GfxMatrix_CreateQuaternionMatrix(
        quaternion, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(quaternionMatrixBytes)));
    auto* quaternionMatrixResult = reinterpret_cast<void*>(
        static_cast<uintptr_t>(quaternionMatrixAddress));

    RwMatrixTransform(
        CSF_AT(uint32_t*, frameMatrixBytes, 0),
        CSF_AT(uint32_t*, quaternionMatrixResult, 0), 2);

    if (CSF_AT(uint8_t, quaternionMatrixBytes, 0x4))
        RwMatrixDestroy(CSF_AT(uint32_t*, quaternionMatrixBytes, 0));

    GfxMatrix_CopyTranslationAndUpdate(
        frameMatrixBytes, nullptr,
        reinterpret_cast<const uint32_t*>(translationBytes));

    float* angles = GfxMatrix_GetEulerAngles(
        reinterpret_cast<float**>(frameMatrixBytes), nullptr,
        reinterpret_cast<float*>(anglesBytes));

    const float pitch = angles[0] * g_RadiansToDegrees;
    const float yaw = angles[1] * g_RadiansToDegrees;
    const float roll = angles[2] * g_RadiansToDegrees;

    self->field_0014 = pitch;
    CSF_AT(float, self, 0x18) = yaw;
    const uint32_t frame = self->field_000c;
    CSF_AT(float, self, 0x1C) = roll;
    RwFrameUpdateObjects(static_cast<int>(frame));

    const char ownsFrameMatrix = CSF_AT(char, frameMatrixBytes, 0x4);
    if (ownsFrameMatrix)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(uint32_t*, frameMatrixBytes, 0)));
    return ownsFrameMatrix;
}

HOOK(0x006BF6A0, C_00860700_ApplyQuaternionRotation, "auto");
