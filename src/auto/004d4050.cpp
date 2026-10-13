#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00863430_GetTargetTransform, 0x00409470, (C_00863430* self, void* , uint32_t* outBuffer));
CSF_FUNC(unsigned int, __fastcall, C_0083bc28_ResetMovementState, 0x004D3830, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083bc28_UpdateWithMovement, 0x004D38A0, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, BddLoader_ReleasePendingHandle, 0x0050E250, (void* self, void*));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(char*, __fastcall, GfxTransform_GetOtherPosition, 0x00655FF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(int, __fastcall, GfxMatrix_CreateQuaternionMatrix, 0x006AE6F0, (float* self, void* , int outputAddress));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionFromAxisAngle, 0x006AE8C0, (float* self, void* , S_00526d40* axis, float angle));
CSF_FUNC(double, __fastcall, C_00863430_GetFacingAngle, 0x00711D70, (void* self, void* edx));

CSF_GLOBAL(float, g_FacingAngleOffset, 0x0082E044);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0083BC04);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);

static int __fastcall C_0083bc28_UpdateAnimationMovement(
    C_0083bc28* self, void* /*edx*/, int animation)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    auto* skin = reinterpret_cast<C_0082dd78*>(
        CSF_AT(uint32_t, owner, 0x4C));
    auto* target =
        reinterpret_cast<C_00863430*(__fastcall*)(void*, void*)>(
            skin->vtbl[0x3C / 4])(skin, nullptr);

    if (g_FacingAngleOffset ==
        std::fabs(C_00863430_GetFacingAngle(target, nullptr)))
    {
        if ((self->field_0044 & 0x14) == 0)
        {
            if (self->field_004c == 5)
            {
                int state = GetGlobalState_50D190();
                BddLoader_ReleasePendingHandle(
                    reinterpret_cast<void*>(state), nullptr);
            }
            C_0083bc28_ResetMovementState(self, nullptr);
            self->field_004c = 1;
        }

        // This call site supplies one stack argument. The generated
        // three-argument vSlot14 wrapper describes a different ABI.
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            target->vtbl[0x14 / 4])(target, nullptr, animation);
    }

    alignas(8) uint8_t beforeQuaternionBytes[0x10];
    alignas(8) uint8_t afterQuaternionBytes[0x10];
    alignas(8) uint8_t directionOutputBytes[0xC];
    alignas(8) uint8_t movementBytes[0x30] = {};
    alignas(8) uint8_t directionBytes[0xC];
    alignas(8) uint8_t matrixHandleBytes[0x8];
    alignas(8) uint8_t vectorBytes[0x10];
    alignas(8) uint8_t positionBytes[0x18];

    auto* beforeQuaternion = reinterpret_cast<float*>(beforeQuaternionBytes);
    auto* afterQuaternion = reinterpret_cast<float*>(afterQuaternionBytes);
    auto* directionOutput = reinterpret_cast<uint32_t*>(directionOutputBytes);
    auto* movement = reinterpret_cast<float*>(movementBytes);
    auto* direction = reinterpret_cast<float*>(directionBytes);
    auto* vector = reinterpret_cast<float*>(vectorBytes);
    auto* transformed = reinterpret_cast<float*>(positionBytes);
    auto* initialPosition = reinterpret_cast<float*>(positionBytes + 0xC);

    CSF_AT(C_0083bc28*, movementBytes, 0x2C) = self;

    float angle = static_cast<float>(
        C_00863430_GetFacingAngle(target, nullptr) * g_DegreesToRadians);
    GfxMatrix_SetQuaternionFromAxisAngle(
        beforeQuaternion, nullptr,
        reinterpret_cast<S_00526d40*>(&g_AxisY), angle);

    auto* position = reinterpret_cast<float*>(
        GfxTransform_GetOtherPosition(target, nullptr));
    auto* targetPosition = reinterpret_cast<float*>(
        C_00863430_GetTargetTransform(
            target, nullptr, reinterpret_cast<uint32_t*>(directionBytes)));

    float differenceZ = targetPosition[2] - position[2];
    float differenceY = targetPosition[1] - position[1];
    vector[0] = targetPosition[0] - position[0];
    vector[1] = differenceY;
    vector[2] = differenceZ;

    int handleAddress = GfxMatrix_CreateQuaternionMatrix(
        beforeQuaternion, nullptr, reinterpret_cast<int>(matrixHandleBytes));
    int matrix = CSF_AT(int, reinterpret_cast<void*>(handleAddress), 0);

    transformed[0] = 0.0f;
    transformed[1] = 0.0f;
    transformed[2] = 0.0f;
    RwV3dTransformVectors(
        reinterpret_cast<int>(transformed), reinterpret_cast<int>(vector),
        1, matrix);

    initialPosition[0] = transformed[0];
    initialPosition[1] = transformed[1];
    initialPosition[2] = transformed[2];
    if (CSF_AT(uint8_t, matrixHandleBytes, 0x4))
    {
        RwMatrixDestroy(
            CSF_AT(std::uint32_t*, matrixHandleBytes, 0));
    }

    position = reinterpret_cast<float*>(
        GfxTransform_GetOtherPosition(target, nullptr));
    initialPosition[0] += position[0];
    initialPosition[1] += position[1];
    initialPosition[2] += position[2];

    reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        target->vtbl[0x14 / 4])(target, nullptr, animation);

    angle = static_cast<float>(
        C_00863430_GetFacingAngle(target, nullptr) * g_DegreesToRadians);
    GfxMatrix_SetQuaternionFromAxisAngle(
        afterQuaternion, nullptr,
        reinterpret_cast<S_00526d40*>(&g_AxisY), angle);

    position = reinterpret_cast<float*>(
        GfxTransform_GetOtherPosition(target, nullptr));
    targetPosition = reinterpret_cast<float*>(
        C_00863430_GetTargetTransform(target, nullptr, directionOutput));

    differenceZ = targetPosition[2] - position[2];
    differenceY = targetPosition[1] - position[1];
    direction[0] = targetPosition[0] - position[0];
    direction[1] = differenceY;
    direction[2] = differenceZ;

    handleAddress = GfxMatrix_CreateQuaternionMatrix(
        afterQuaternion, nullptr, reinterpret_cast<int>(matrixHandleBytes));
    matrix = CSF_AT(int, reinterpret_cast<void*>(handleAddress), 0);

    vector[0] = 0.0f;
    vector[1] = 0.0f;
    vector[2] = 0.0f;
    RwV3dTransformVectors(
        reinterpret_cast<int>(vector), reinterpret_cast<int>(direction),
        1, matrix);

    transformed[1] = vector[1];
    transformed[0] = vector[0];
    transformed[2] = vector[2];
    if (CSF_AT(uint8_t, matrixHandleBytes, 0x4))
    {
        RwMatrixDestroy(
            CSF_AT(std::uint32_t*, matrixHandleBytes, 0));
    }

    position = reinterpret_cast<float*>(
        GfxTransform_GetOtherPosition(target, nullptr));
    float finalX = transformed[0] + position[0];
    float finalY = transformed[1] + position[1];
    transformed[2] += position[2];

    direction[0] = finalX - initialPosition[0];
    direction[1] = finalY - initialPosition[1];
    direction[2] = transformed[2] - initialPosition[2];

    auto* convertedDirection = reinterpret_cast<float*>(
        S_00526d40_ConvertDirection(
            reinterpret_cast<S_00526d40*>(directionBytes),
            nullptr, directionOutput));
    float directionX = convertedDirection[0];
    float directionY = convertedDirection[1];
    float directionZ = convertedDirection[2];

    movement[0] = initialPosition[0];
    movement[3] = directionX;
    movement[1] = initialPosition[1];
    movement[2] = initialPosition[2];
    movement[4] = directionY;
    movement[5] = directionZ;
    CSF_AT(uint8_t, movementBytes, 0x28) = 1;

    return C_0083bc28_UpdateWithMovement(self, nullptr, animation, movement);
}

HOOK(0x004D4050, C_0083bc28_UpdateAnimationMovement, "auto");
