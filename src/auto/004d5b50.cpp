#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, Vehicle_AimAtPosition, 0x0041C960, (void* self, void* , float* position, char interpolate));
CSF_FUNC(uint32_t*, __fastcall, C_00833708_SendOrder, 0x0048A4D0, (C_00833708* self, void* , int taskIndex, int orderAddress));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083c280_SetTaskStateCode, 0x004D5D90, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

CSF_GLOBAL(float, g_WeaponPlacementTraceDepth, 0x0082E11C);

static char __fastcall C_0083c280_AimAtAimPointAndSendOrder(
    C_0083c280* self, void* /*edx*/)
{
    alignas(8) uint8_t matrixHandleBytes[0x8];
    alignas(8) uint8_t aimPositionBytes[0x1C];
    alignas(8) uint8_t orderBytes[0x24];

    void* characterObject = reinterpret_cast<void*>(self->field_0014);
    void* aimController = CSF_AT(void*, characterObject, 0x4C);

    void* relatedObject =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, characterObject, 0)[0x28 / 4])(
                characterObject, nullptr);
    void* transformObject =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, relatedObject, 0)[0x18 / 4])(
                relatedObject, nullptr);
    void* matrixHandle =
        ((void*(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, transformObject, 0)[0x8C / 4])(
                transformObject, nullptr, matrixHandleBytes);

    void* matrix = CSF_AT(void*, matrixHandle, 0);
    float forwardX = CSF_AT(float, matrix, 0x20);
    float forwardY = CSF_AT(float, matrix, 0x24);
    float forwardZ = CSF_AT(float, matrix, 0x28);

    if (CSF_AT(uint8_t, matrixHandleBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixHandleBytes, 0));

    void* positionObject = reinterpret_cast<void*>(self->field_0014);
    float offsetX = forwardX * g_WeaponPlacementTraceDepth;
    float offsetY = forwardY * g_WeaponPlacementTraceDepth;
    float offsetZ = forwardZ * g_WeaponPlacementTraceDepth;
    float* position = GetObjectPositionVector(positionObject, nullptr);

    float targetZ = offsetZ + position[2];
    float targetY = offsetY + position[1];
    CSF_AT(float, aimPositionBytes, 0x0) = offsetX + position[0];
    CSF_AT(float, aimPositionBytes, 0x4) = targetY;
    CSF_AT(float, aimPositionBytes, 0x8) = targetZ;

    Vehicle_AimAtPosition(
        aimController, nullptr, reinterpret_cast<float*>(aimPositionBytes), 1);

    CSF_AT(int, orderBytes, 0xC) = -1;
    CSF_AT(int, orderBytes, 0x10) = -1;
    CSF_AT(uint32_t, orderBytes, 0x4) = 0;
    CSF_AT(void**, orderBytes, 0x8) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, orderBytes, 0x14) = 0;
    CSF_AT(uint32_t, orderBytes, 0x18) = 0;
    CSF_AT(uint32_t, orderBytes, 0x1C) = 0;
    CSF_AT(uint8_t, orderBytes, 0x20) = 1;
    CSF_AT(uint8_t, orderBytes, 0x21) = 0;
    CSF_AT(uint8_t, orderBytes, 0x22) = 1;
    CSF_AT(int, orderBytes, 0x0) = 2;

    // The original reloads self->field_0014 after Vehicle_AimAtPosition,
    // so the SendOrder target is read here, not taken from the cached pointer.
    C_00833708_SendOrder(
        reinterpret_cast<C_00833708*>(CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0x44)),
        nullptr, 0, reinterpret_cast<int>(orderBytes));

    return C_0083c280_SetTaskStateCode(self, nullptr, 6);
}

HOOK(0x004D5B50, C_0083c280_AimAtAimPointAndSendOrder, "auto");
