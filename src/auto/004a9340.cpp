#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, AiSearchOrders_AimControllerAtPosition, 0x0040CA40, (void* self, void* edx, float* arg0, char a4));
CSF_FUNC(uint32_t*, __fastcall, C_00833708_SendOrder, 0x0048A4D0, (C_00833708* self, void* , int taskIndex, int orderAddress));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00835228_SetStateDispatch, 0x004A9720, (C_00835228* self, void* edx, uint32_t newState));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

CSF_GLOBAL(float, g_WeaponPlacementTraceDepth, 0x0082E11C);

static int __fastcall C_00835228_AimForwardAndSendOrder(
    C_00835228* self, void* /*edx*/)
{
    alignas(8) uint8_t matrixHandleBytes[0x8];
    alignas(8) uint8_t aimPositionBytes[0x1C];
    alignas(8) uint8_t orderBytes[0x24];

    auto* character = reinterpret_cast<C_00833908*>(self->field_0014);
    void* aimController = reinterpret_cast<void*>(character->field_004c);

    void* relatedObject =
        reinterpret_cast<void*>(character->vGetField016c());
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
    CSF_AT(float, aimPositionBytes, 0) = offsetX + position[0];
    CSF_AT(float, aimPositionBytes, 0x4) = targetY;
    CSF_AT(float, aimPositionBytes, 0x8) = targetZ;

    AiSearchOrders_AimControllerAtPosition(
        aimController, nullptr, reinterpret_cast<float*>(aimPositionBytes), 1);

    CSF_AT(int, orderBytes, 0xC) = -1;
    CSF_AT(int, orderBytes, 0x10) = -1;
    character = reinterpret_cast<C_00833908*>(self->field_0014);
    CSF_AT(uint32_t, orderBytes, 0x4) = 0;
    CSF_AT(void**, orderBytes, 0x8) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, orderBytes, 0x14) = 0;
    CSF_AT(uint32_t, orderBytes, 0x18) = 0;
    CSF_AT(uint32_t, orderBytes, 0x1C) = 0;
    CSF_AT(uint8_t, orderBytes, 0x20) = 1;
    CSF_AT(uint8_t, orderBytes, 0x21) = 0;
    CSF_AT(uint8_t, orderBytes, 0x22) = 1;
    CSF_AT(int, orderBytes, 0) = 2;

    C_00833708_SendOrder(
        reinterpret_cast<C_00833708*>(character->field_0044),
        nullptr, 0, reinterpret_cast<int>(orderBytes));

    return C_00835228_SetStateDispatch(self, nullptr, 4);
}

HOOK(0x004A9340, C_00835228_AimForwardAndSendOrder, "auto");
