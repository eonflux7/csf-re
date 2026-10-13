#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, Vehicle_AimPrimaryEntryAtPosition, 0x0040DAB0, (void* self, void* edx, float* position, char interpolate));
CSF_FUNC(uint32_t*, __fastcall, C_00833708_SendOrder, 0x0048A4D0, (C_00833708* self, void* edx, int taskIndex, int orderAddress));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083ac40_DispatchStateCode, 0x004CE350, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

CSF_GLOBAL(float, g_WeaponPlacementTraceDepth, 0x0082E11C);

static char __fastcall C_0083ac40_AimVehicleForwardAndSendOrder(
    C_0083ac40* self, void* /*edx*/)
{
    alignas(8) uint8_t matrixHandleBytes[0x8];
    alignas(8) uint8_t aimPositionBytes[0x1C];
    alignas(8) uint8_t orderBytes[0x24];

    void* character = reinterpret_cast<void*>(self->field_0014);
    void* aimController = reinterpret_cast<void*>(CSF_AT(uint32_t, character, 0x4C));

    void* relatedObject =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, character, 0)[0x28 / 4])(character, nullptr);
    void* transformObject =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, relatedObject, 0)[0x18 / 4])(relatedObject, nullptr);
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

    float scaledX = forwardX * g_WeaponPlacementTraceDepth;
    float scaledY = forwardY * g_WeaponPlacementTraceDepth;
    float scaledZ = forwardZ * g_WeaponPlacementTraceDepth;

    void* positionObject = reinterpret_cast<void*>(self->field_0014);
    float* position = GetObjectPositionVector(positionObject, nullptr);

    CSF_AT(float, aimPositionBytes, 0x0) = scaledX + position[0];
    CSF_AT(float, aimPositionBytes, 0x4) = scaledY + position[1];
    CSF_AT(float, aimPositionBytes, 0x8) = scaledZ + position[2];

    Vehicle_AimPrimaryEntryAtPosition(
        aimController, nullptr, reinterpret_cast<float*>(aimPositionBytes), 1);

    CSF_AT(int, orderBytes, 0x4) = 0;
    CSF_AT(void**, orderBytes, 0x8) = CSF_VTBL(0x0082D448);
    CSF_AT(int, orderBytes, 0xC) = -1;
    CSF_AT(int, orderBytes, 0x10) = -1;
    CSF_AT(uint32_t, orderBytes, 0x14) = 0;
    CSF_AT(uint32_t, orderBytes, 0x18) = 0;
    CSF_AT(uint32_t, orderBytes, 0x1C) = 0;
    CSF_AT(uint8_t, orderBytes, 0x20) = 1;
    CSF_AT(uint8_t, orderBytes, 0x21) = 0;
    CSF_AT(uint8_t, orderBytes, 0x22) = 1;
    CSF_AT(int, orderBytes, 0x0) = 2;

    C_00833708_SendOrder(
        reinterpret_cast<C_00833708*>(CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 0x44)),
        nullptr, 0, reinterpret_cast<int>(orderBytes));

    return C_0083ac40_DispatchStateCode(reinterpret_cast<void*>(self), nullptr, 6);
}

HOOK(0x004CDF30, C_0083ac40_AimVehicleForwardAndSendOrder, "auto");
