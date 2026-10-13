#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00863430_GetTargetTransform, 0x00409470, (C_00863430* self, void* , uint32_t* outBuffer));
CSF_FUNC(void, __fastcall, C_0083bc28_ResetAnimationState, 0x004D3870, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083bc28_ApplyAnimationDisplacement, 0x004D38A0, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, BddLoader_ReleasePendingHandle, 0x0050E250, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(char*, __fastcall, GfxTransform_GetOtherPosition, 0x00655FF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(int, __fastcall, GfxMatrix_CreateQuaternionMatrix, 0x006AE6F0, (float* self, void* , int outputAddress));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionFromAxisAngle, 0x006AE8C0, (float* self, void* , S_00526d40* axis, float angle));
CSF_FUNC(double, __fastcall, C_00863430_GetRotationAngle, 0x00711D70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_AnimationDegreesToRadians, 0x0083BC04);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);

static int __fastcall C_0083bc28_SetAnimationWithDisplacement(
    C_0083bc28* self, void* /*edx*/, int animation)
{
    auto* owner = reinterpret_cast<C_0083b958*>(self->field_0014);
    void* skin = CSF_AT(void*, owner, 0x4C);
    auto* target = reinterpret_cast<C_00863430*>(
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, skin, 0)[0x3C / 4])(skin, nullptr));

    // The sound header and its payload occupy consecutive stack storage.
    alignas(8) uint8_t soundBytes[0xC + 0x2C]{};
    uint8_t* payload = soundBytes + 0xC;

    if (g_fZero == C_00863430_GetRotationAngle(target, nullptr))
    {
        CSF_AT(int, payload, 0x1C) = -1;
        CSF_AT(int, payload, 0x20) = -1;
        CSF_AT(int, soundBytes, 0x08) = -1;
        CSF_AT(int, payload, 0x00) = -1;

        owner = reinterpret_cast<C_0083b958*>(self->field_0014);
        CSF_AT(int, payload, 0x0C) = 0;
        CSF_AT(int, payload, 0x10) = 0;
        CSF_AT(int, payload, 0x14) = 0;
        CSF_AT(void**, payload, 0x18) = CSF_VTBL(0x0082D448);
        CSF_AT(float, payload, 0x04) = 0.0f;
        CSF_AT(float, payload, 0x08) = 0.0f;
        CSF_AT(uint8_t, payload, 0x28) = 0;
        CSF_AT(uint8_t, payload, 0x29) = 1;
        CSF_AT(uint8_t, payload, 0x2A) = 1;
        CSF_AT(int, payload, 0x24) = 0;
        CSF_AT(int, soundBytes, 0x00) = 2;
        CSF_AT(int, soundBytes, 0x04) = 2;
        CSF_AT(uint32_t, payload, 0x1C) = owner->field_0020;
        CSF_AT(uint32_t, payload, 0x20) = owner->field_0024;

        int scene = GetGlobalValue_8BC1E0();
        CSF_AT(int, soundBytes, 0x08) = SceneLoader_GetResourceById(
            reinterpret_cast<uint8_t*>(scene) + 0x22A4, nullptr, 32);
        auto* soundManager =
            reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
        S_00521df0_PlaySoundRequest(soundManager, nullptr, soundBytes, -1.0f);

        if ((self->field_0044 & 0x10) == 0)
        {
            if (self->field_004c == 5)
            {
                int state = GetGlobalState_50D190();
                BddLoader_ReleasePendingHandle(
                    reinterpret_cast<void*>(state), nullptr);
            }
            C_0083bc28_ResetAnimationState(self, nullptr);
            self->field_004c = 2;
        }

        // This call site supplies one stack argument; vSlot14's generated
        // three-argument signature does not describe this target's slot.
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            target->vtbl[0x14 / 4])(target, nullptr, animation);
    }

    alignas(8) uint8_t beforeQuaternionBytes[0x10];
    alignas(8) uint8_t afterQuaternionBytes[0x10];
    alignas(8) uint8_t directionBytes[0xC];
    alignas(8) uint8_t matrixBytes[0x8];
    alignas(8) uint8_t targetVectorBytes[0xC];
    alignas(8) uint8_t rotatedVectorBytes[0x10];
    alignas(8) uint8_t positionsBytes[0x18];

    auto* beforeQuaternion = reinterpret_cast<float*>(beforeQuaternionBytes);
    auto* afterQuaternion = reinterpret_cast<float*>(afterQuaternionBytes);
    auto* targetVector = reinterpret_cast<float*>(targetVectorBytes);
    auto* rotatedVector = reinterpret_cast<float*>(rotatedVectorBytes);
    auto* positions = reinterpret_cast<float*>(positionsBytes);

    float angle = static_cast<float>(
        C_00863430_GetRotationAngle(target, nullptr) *
        g_AnimationDegreesToRadians);
    GfxMatrix_SetQuaternionFromAxisAngle(
        beforeQuaternion, nullptr,
        reinterpret_cast<S_00526d40*>(&g_AxisY), angle);

    auto* origin = reinterpret_cast<float*>(
        GfxTransform_GetOtherPosition(target, nullptr));
    auto* targetPosition = reinterpret_cast<float*>(
        C_00863430_GetTargetTransform(
            target, nullptr, reinterpret_cast<uint32_t*>(targetVectorBytes)));

    float differenceZ = targetPosition[2] - origin[2];
    float differenceY = targetPosition[1] - origin[1];
    rotatedVector[0] = targetPosition[0] - origin[0];
    rotatedVector[1] = differenceY;
    rotatedVector[2] = differenceZ;

    int matrixResult = GfxMatrix_CreateQuaternionMatrix(
        beforeQuaternion, nullptr, reinterpret_cast<int>(matrixBytes));
    int matrix = CSF_AT(int, reinterpret_cast<void*>(matrixResult), 0);
    positions[0] = 0.0f;
    positions[1] = 0.0f;
    positions[2] = 0.0f;
    RwV3dTransformVectors(
        reinterpret_cast<int>(positions), reinterpret_cast<int>(rotatedVector),
        1, matrix);

    positions[3] = positions[0];
    positions[4] = positions[1];
    positions[5] = positions[2];
    if (CSF_AT(uint8_t, matrixBytes, 0x04))
        RwMatrixDestroy(CSF_AT(uint32_t*, matrixBytes, 0));

    origin = reinterpret_cast<float*>(
        GfxTransform_GetOtherPosition(target, nullptr));
    positions[3] = positions[3] + origin[0];
    positions[4] = positions[4] + origin[1];
    positions[5] = positions[5] + origin[2];

    reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        target->vtbl[0x14 / 4])(target, nullptr, animation);

    angle = static_cast<float>(
        C_00863430_GetRotationAngle(target, nullptr) *
        g_AnimationDegreesToRadians);
    GfxMatrix_SetQuaternionFromAxisAngle(
        afterQuaternion, nullptr,
        reinterpret_cast<S_00526d40*>(&g_AxisY), angle);

    origin = reinterpret_cast<float*>(
        GfxTransform_GetOtherPosition(target, nullptr));
    targetPosition = reinterpret_cast<float*>(
        C_00863430_GetTargetTransform(
            target, nullptr, reinterpret_cast<uint32_t*>(directionBytes)));

    differenceZ = targetPosition[2] - origin[2];
    differenceY = targetPosition[1] - origin[1];
    targetVector[0] = targetPosition[0] - origin[0];
    targetVector[1] = differenceY;
    targetVector[2] = differenceZ;

    matrixResult = GfxMatrix_CreateQuaternionMatrix(
        afterQuaternion, nullptr, reinterpret_cast<int>(matrixBytes));
    matrix = CSF_AT(int, reinterpret_cast<void*>(matrixResult), 0);
    rotatedVector[0] = 0.0f;
    rotatedVector[1] = 0.0f;
    rotatedVector[2] = 0.0f;
    RwV3dTransformVectors(
        reinterpret_cast<int>(rotatedVector), reinterpret_cast<int>(targetVector),
        1, matrix);

    positions[0] = rotatedVector[0];
    positions[1] = rotatedVector[1];
    positions[2] = rotatedVector[2];
    if (CSF_AT(uint8_t, matrixBytes, 0x04))
        RwMatrixDestroy(CSF_AT(uint32_t*, matrixBytes, 0));

    origin = reinterpret_cast<float*>(
        GfxTransform_GetOtherPosition(target, nullptr));
    float newX = positions[0] + origin[0];
    float newY = positions[1] + origin[1];
    positions[2] = positions[2] + origin[2];
    targetVector[0] = newX - positions[3];
    targetVector[1] = newY - positions[4];
    targetVector[2] = positions[2] - positions[5];

    uint32_t* direction = S_00526d40_ConvertDirection(
        reinterpret_cast<S_00526d40*>(targetVectorBytes), nullptr,
        reinterpret_cast<uint32_t*>(directionBytes));
    uint32_t directionX = direction[0];
    uint32_t directionY = direction[1];
    uint32_t directionZ = direction[2];

    CSF_AT(float, payload, 0x00) = positions[3];
    CSF_AT(uint32_t, payload, 0x10) = directionY;
    CSF_AT(uint32_t, payload, 0x14) = directionZ;
    CSF_AT(float, payload, 0x04) = positions[4];
    CSF_AT(uint32_t, payload, 0x0C) = directionX;
    CSF_AT(float, payload, 0x08) = positions[5];
    CSF_AT(uint8_t, payload, 0x28) = 1;
    return C_0083bc28_ApplyAnimationDisplacement(
        self, nullptr, animation, reinterpret_cast<float*>(payload));
}

HOOK(0x004D42D0, C_0083bc28_SetAnimationWithDisplacement, "auto");
