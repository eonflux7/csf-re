#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ContextualDistanceScale, 0x0082DD74);

CSF_FUNC(double, __fastcall, Vector_GetLength, 0x00401FA0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(unsigned int, __fastcall, CCharActionFsm_CheckContextualAction, 0x00485CA0, (void* self, void* edx, uint8_t* a3, uint8_t* a4));
CSF_FUNC(int, __fastcall, CCharActionFsm_FinishAlternateSelection, 0x004876A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesSecondCheck, 0x0048E160, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Hud_GetAlternateSceneSelection, 0x004950B0, (void* self, void* edx));
CSF_FUNC(int32_t, __fastcall, C_00495560_InnerTypeIs38, 0x00495560, (void* self, void*));
CSF_FUNC(int, __fastcall, CharActionFsm_ClearContextualSelection, 0x00498840, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_SetActionFlag, 0x00498F50, (void* self, void* edx, int a2, char a3));
CSF_FUNC(uint32_t, __fastcall, SceneObject_ApplyAlternateSelection, 0x0049BF50, (void* self, void* edx, void* target));
CSF_FUNC(int, __fastcall, IsField48LessThanField44, 0x004CF380, (void* self, void*));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (void* self, void* , int value));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall CCharActionFsm_ExecuteContextualAction(
    CCharActionFsm* self, void* /*edx*/)
{
    auto* action = reinterpret_cast<uint8_t*>(self->field_0008) + 0x298;
    self->field_00bc = 1;

    if (CSF_AT(int32_t, action, 0xC) < 0)
        return self->vBeginWeaponStateChange(0, 0);

    auto* target = reinterpret_cast<uint8_t*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr,
                            reinterpret_cast<int>(action + 4)));
    if (!target)
        return self->vBeginWeaponStateChange(0, 0);

    SceneObject_SetActionFlag(
        reinterpret_cast<void*>(self->field_0008), nullptr, 32, 1);

    switch (CSF_AT(uint32_t, action, 0))
    {
    case 1:
        self->vSetStateAndDispatch(11);
        break;

    case 2:
        self->field_0044 = CSF_AT(uint32_t, target, 0x20);
        self->field_0048 = CSF_AT(uint32_t, target, 0x24);
        self->vSetStateAndDispatch(10);
        break;

    case 3:
    {
        auto* ownerPosition = reinterpret_cast<float*>(
            GetObjectPositionVector(
                reinterpret_cast<void*>(self->field_0008), nullptr));
        float facing[3] = {};
        auto* targetPosition = reinterpret_cast<void*>(
            GetObjectPositionVector(target, nullptr));
        float* direction = BuildFacingVectorBetween(
            targetPosition, nullptr, facing, ownerPosition);
        float distance = static_cast<float>(Vector_GetLength(direction, nullptr));

        auto* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        if (CSF_AT(float, settings, 0xB04) * g_ContextualDistanceScale > distance)
        {
            alignas(4) uint8_t handle[0x16] = {};
            CSF_AT(int32_t, handle, 4) = -1;
            CSF_AT(int32_t, handle, 8) = -1;
            auto* ownerHandle =
                reinterpret_cast<uint8_t*>(self->field_0008) + 0x1C;
            CSF_AT(void**, handle, 0) = CSF_VTBL(0x0082D448);
            CSF_AT(uint32_t, handle, 4) = CSF_AT(uint32_t, ownerHandle, 4);
            CSF_AT(uint32_t, handle, 8) = CSF_AT(uint32_t, ownerHandle, 8);
            CSF_AT(uint8_t, handle, 0xC) = 0;

            auto* receiver = target + 4;
            ((void (__fastcall*)(void*, void*, void*))
                CSF_AT(void**, receiver, 0)[0x3C / 4])(
                    receiver, nullptr, handle);
            self->vBeginWeaponStateChange(0, 0);
        }
        else
        {
            self->vBeginWeaponStateChange(18, 0);
        }
        break;
    }

    case 4:
    {
        uint8_t available = 0;
        uint8_t requiresStateChange = 0;
        CCharActionFsm_CheckContextualAction(
            self, nullptr, &available, &requiresStateChange);
        if (!available)
        {
            self->vBeginWeaponStateChange(0, 0);
            break;
        }

        void* missionInfo = reinterpret_cast<void*>(Get_008b0678());
        MenuMissionInfo_SetGlobalIndex(missionInfo, nullptr, 94);
        self->field_00cc = 2.0f;
        if (requiresStateChange)
        {
            self->field_0070 = 1;
            self->vBeginWeaponStateChange(4, 0);
        }
        else
        {
            self->vBeginWeaponStateChange(0, 0);
        }
        break;
    }

    case 5:
    {
        int alternateSelection = Hud_GetAlternateSceneSelection(
            reinterpret_cast<void*>(self->field_0008), nullptr);
        SceneObject_ApplyAlternateSelection(
            reinterpret_cast<void*>(self->field_0008), nullptr, target);
        if (alternateSelection)
            CCharActionFsm_FinishAlternateSelection(self, nullptr);
        else
            self->vBeginWeaponStateChange(0, 0);
        break;
    }

    case 6:
    {
        alignas(4) uint8_t handle[0x12] = {};
        CSF_AT(int32_t, handle, 4) = -1;
        CSF_AT(int32_t, handle, 8) = -1;
        auto* ownerHandle =
            reinterpret_cast<uint8_t*>(self->field_0008) + 0x1C;
        CSF_AT(void**, handle, 0) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, handle, 4) = CSF_AT(uint32_t, ownerHandle, 4);
        CSF_AT(uint32_t, handle, 8) = CSF_AT(uint32_t, ownerHandle, 8);

        auto* receiver = target + 4;
        ((void (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, receiver, 0)[0x40 / 4])(
                receiver, nullptr, handle);
        self->vBeginWeaponStateChange(0, 0);
        break;
    }

    case 8:
        if ((CSF_AT(uint8_t, target, 0x94) & 1) != 0)
        {
            self->field_00b4 = CSF_AT(uint32_t, target, 0x20);
            self->field_00b8 = CSF_AT(uint32_t, target, 0x24);
            ((void (__fastcall*)(void*, void*, uint32_t*))
                CSF_AT(void**, target, 0)[0x68 / 4])(
                    target, nullptr, nullptr);
            self->vBeginWeaponStateChange(17, 0);
        }
        else
        {
            self->vBeginWeaponStateChange(0, 0);
        }
        break;

    case 9:
        if (static_cast<uint8_t>(SceneObject_PassesFirstCheck(target, nullptr)) &&
            static_cast<uint8_t>(SceneObject_PassesSecondCheck(target, nullptr)))
            self->vSetStateAndDispatch(33);
        else
            self->vBeginWeaponStateChange(0, 0);
        break;

    case 0xA:
        self->vSetStateAndDispatch(37);
        break;

    case 0xB:
        self->vSetStateAndDispatch(36);
        break;

    case 0xC:
        if (!static_cast<uint8_t>(C_00495560_InnerTypeIs38(
                reinterpret_cast<void*>(self->field_0008), nullptr)))
        {
            if (static_cast<uint8_t>(IsField48LessThanField44(
                    reinterpret_cast<void*>(CSF_AT(uint32_t, target, 0x34)),
                    nullptr)))
                self->vSetStateAndDispatch(38);
            else
                self->vBeginWeaponStateChange(0, 0);
        }
        break;

    case 0xD:
        self->vSetStateAndDispatch(39);
        break;

    case 0xE:
        self->vSetStateAndDispatch(40);
        break;

    case 0xF:
        self->vSetStateAndDispatch(41);
        break;

    case 0x10:
    {
        alignas(4) uint8_t request[0x12] = {};
        CSF_AT(uint32_t, request, 4) = self->field_0008;
        CSF_AT(int32_t, request, 0) = 2;
        CSF_AT(int32_t, request, 8) = 0;

        auto* receiver = target + 4;
        ((void (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, receiver, 0)[0x48 / 4])(
                receiver, nullptr, request);
        self->vBeginWeaponStateChange(0, 0);
        break;
    }

    default:
        self->vBeginWeaponStateChange(0, 0);
        break;
    }

    return CharActionFsm_ClearContextualSelection(
        reinterpret_cast<void*>(self->field_0008), nullptr);
}

HOOK(0x00487C40, CCharActionFsm_ExecuteContextualAction, "auto");
