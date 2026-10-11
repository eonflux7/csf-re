#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083eb70_IsTargetExcluded, 0x004E73F0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_0083eb70_CanUseAlternateAction, 0x004E7E30, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_0083eb70_ValidateAction, 0x004E7F10, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083eb70_BuildTargetAction(
    C_0083eb70* self, void* /*edx*/, int targetHandle, int requestedAction, int resultAddress)
{
    void* result = reinterpret_cast<void*>(resultAddress);
    CSF_AT(int, result, 0x04) = -1;

    if (C_0083eb70_IsTargetExcluded(self, nullptr, targetHandle))
        return CSF_AT(int, result, 0x04);

    void* target = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, targetHandle));
    if (!target)
        return CSF_AT(int, result, 0x04);

    void* handle = reinterpret_cast<void*>(targetHandle);
    CSF_AT(uint32_t, result, 0x18) = CSF_AT(uint32_t, handle, 0x04);
    CSF_AT(uint32_t, result, 0x1C) = CSF_AT(uint32_t, handle, 0x08);

    auto* owner = reinterpret_cast<C_00833908*>(self->field_0008);
    if (owner->field_0050)
    {
        const uint32_t ownerState = CSF_AT(uint32_t, owner, 0x1D8);
        switch (requestedAction)
        {
        case 14:
            if (self->vSlot9C(targetHandle, 13))
            {
                CSF_AT(int, result, 0x04) = 3;
                CSF_AT(int, result, 0x08) = 13;
            }
            else if (self->vSlot9C(targetHandle, 14))
            {
                CSF_AT(int, result, 0x04) = 3;
                CSF_AT(int, result, 0x08) = 14;
            }
            else if (C_0083eb70_CanUseAlternateAction(self, nullptr, targetHandle))
            {
                CSF_AT(int, result, 0x04) = 3;
                CSF_AT(int, result, 0x08) = 12;
            }
            else if (ownerState != 3 && ownerState != 2 &&
                     self->vSlot9C(targetHandle, 3))
            {
                CSF_AT(int, result, 0x04) = 1;
                CSF_AT(int, result, 0x08) = 3;
            }
            break;

        case 11:
            if (self->vSlot9C(targetHandle, 11))
            {
                CSF_AT(int, result, 0x04) = 3;
                CSF_AT(int, result, 0x08) = 11;
            }
            break;

        case 10:
            if (self->vSlot9C(targetHandle, 10))
            {
                CSF_AT(int, result, 0x04) = 3;
                CSF_AT(int, result, 0x08) = 10;
            }
            break;

        case 5:
            if (ownerState != 3 && self->vSlot9C(targetHandle, 5))
            {
                CSF_AT(int, result, 0x04) = 2;
                CSF_AT(int, result, 0x08) = 5;
            }
            break;

        case 6:
            if (ownerState == 3)
            {
                if (self->vSlot9C(targetHandle, 6))
                {
                    void* resolvedTarget = reinterpret_cast<void*>(
                        CHandleTable_Resolve(g_pCurrentScene, nullptr, targetHandle));
                    if (resolvedTarget)
                    {
                        const int targetType = CSF_AT(int, resolvedTarget, 0x50);
                        if (targetType == 8 || targetType == 16)
                        {
                            CSF_AT(int, result, 0x04) = 2;
                            CSF_AT(int, result, 0x08) = 6;
                        }
                    }
                }
            }
            else if (self->vSlot9C(targetHandle, 6))
            {
                CSF_AT(int, result, 0x04) = 2;
                CSF_AT(int, result, 0x08) = 6;
            }
            break;

        default:
            if (requestedAction == 1 && ownerState != 3 &&
                ownerState != 2 && ownerState != 1 &&
                self->vSlot9C(targetHandle, 1))
            {
                CSF_AT(int, result, 0x04) = 0;
                CSF_AT(int, result, 0x08) = 1;
            }
            break;
        }
    }
    else if ((CSF_AT(uint32_t, target, 0x170) & 0x2000) != 0 ||
             static_cast<uint8_t>(
                 GameConfig_IsResolvedEntryActive(CSF_AT(int, target, 0x50))) != 0)
    {
        CSF_AT(int, result, 0x04) = 3;
        CSF_AT(int, result, 0x08) = 14;
    }

    if (CSF_AT(int, result, 0x04) != -1)
    {
        float* position = GetObjectPositionVector(target, nullptr);
        CSF_AT(uint32_t, result, 0x20) = CSF_AT(uint32_t, position, 0x00);
        CSF_AT(uint32_t, result, 0x24) = CSF_AT(uint32_t, position, 0x04);
        CSF_AT(uint32_t, result, 0x28) = CSF_AT(uint32_t, position, 0x08);

        float* targetPosition = GetObjectPositionVector(target, nullptr);
        float* ownerPosition = GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0008), nullptr);
        const float dx = ownerPosition[0] - targetPosition[0];
        const float dy = ownerPosition[1] - targetPosition[1];
        const float dz = ownerPosition[2] - targetPosition[2];
        CSF_AT(float, result, 0x2C) = dz * dz + dx * dx + dy * dy;

        if (!C_0083eb70_ValidateAction(self, nullptr, resultAddress))
            CSF_AT(int, result, 0x04) = -1;
    }

    return CSF_AT(int, result, 0x04);
}

HOOK(0x004E8390, C_0083eb70_BuildTargetAction, "auto");
