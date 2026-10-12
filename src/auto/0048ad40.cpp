#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_TransformVector3, 0x004069F0, (S_00526d40* self, void* , uint32_t* out, int* in));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_CreateQuaternionMatrix, 0x006AE6F0, (float* self, void* , int outputAddress));
CSF_FUNC(float*, __fastcall, GfxMatrix_MultiplyQuaternionInPlace, 0x006AE830, (float* self, void* , float* a2));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionFromAxisAngle, 0x006AE8C0, (float* self, void* , S_00526d40* axis, float angle));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_FullTurnAngle, 0x00833804);
CSF_GLOBAL(float, g_MinimumRotationAngle, 0x00833808);

static char __cdecl S_00526d40_RotateVectorWithStateAngle(
    S_00526d40* vector, float angle, uint32_t* out)
{
    // Unordered comparisons take the rotation path, just like the original.
    if (angle < g_MinimumRotationAngle)
    {
        out[0] = vector[0].field_0000;
        out[1] = vector[1].field_0000;
        const uint32_t lastComponent = vector[2].field_0000;
        out[2] = lastComponent;
        return static_cast<char>(lastComponent);
    }

    alignas(8) uint8_t tiltQuaternionBytes[0x10];
    alignas(8) uint8_t spinQuaternionBytes[0x10];
    alignas(8) uint8_t transformedBytes[0xC];
    alignas(8) uint8_t axisBytes[0xC];
    alignas(8) uint8_t matrixOutputBytes[0x8];

    auto* tiltQuaternion = reinterpret_cast<float*>(tiltQuaternionBytes);
    auto* spinQuaternion = reinterpret_cast<float*>(spinQuaternionBytes);
    S_00526d40& axis = *reinterpret_cast<S_00526d40*>(axisBytes);

    const float zero = g_fZero;
    if (reinterpret_cast<const float&>(vector[0].field_0000) == zero &&
        reinterpret_cast<const float&>(vector[1].field_0000) == zero)
    {
        reinterpret_cast<float&>(axis.field_0000) = zero;
        CSF_AT(uint32_t, axisBytes, 0x8) = vector[1].field_0000;
        CSF_AT(float, axisBytes, 0x4) =
            -reinterpret_cast<const float&>(vector[2].field_0000);
    }
    else
    {
        CSF_AT(uint32_t, axisBytes, 0x4) = vector[0].field_0000;
        reinterpret_cast<float&>(axis.field_0000) =
            -reinterpret_cast<const float&>(vector[1].field_0000);
        CSF_AT(float, axisBytes, 0x8) = zero;
    }

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    const int stateValue = GetStateValue(state, nullptr, 100);
    CSF_AT(float, matrixOutputBytes, 0x4) =
        static_cast<float>(stateValue) * g_ProgressScale * g_FullTurnAngle;

    GfxMatrix_SetQuaternionFromAxisAngle(
        tiltQuaternion, nullptr, &axis, angle);
    GfxMatrix_SetQuaternionFromAxisAngle(
        spinQuaternion, nullptr, vector,
        CSF_AT(float, matrixOutputBytes, 0x4));
    GfxMatrix_MultiplyQuaternionInPlace(
        spinQuaternion, nullptr, tiltQuaternion);

    const int matrix = GfxMatrix_CreateQuaternionMatrix(
        spinQuaternion, nullptr, reinterpret_cast<int>(matrixOutputBytes));
    uint32_t* transformed = S_00526d40_TransformVector3(
        vector, nullptr, reinterpret_cast<uint32_t*>(transformedBytes),
        reinterpret_cast<int*>(matrix));

    out[0] = transformed[0];
    out[1] = transformed[1];
    out[2] = transformed[2];

    const uint8_t ownsMatrix = CSF_AT(uint8_t, matrixOutputBytes, 0x4);
    if (ownsMatrix)
    {
        return static_cast<char>(RwMatrixDestroy(
            CSF_AT(std::uint32_t*, matrixOutputBytes, 0x0)));
    }
    return static_cast<char>(ownsMatrix);
}

HOOK(0x0048AD40, S_00526d40_RotateVectorWithStateAngle, "auto");
