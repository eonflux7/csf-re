#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(bool, __fastcall, AiSearchOrders_AimLinkedObjectAtPosition, 0x0040CA40, (void* self, void* edx, float* arg0, char a4));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void*, __fastcall, AttackTasks_InitTaskBase, 0x0047C4D0, (void* self, void* , uint32_t owner));
CSF_FUNC(char, __fastcall, CharActionFsm_CopyParamsFromSource, 0x0047FAB0, (void* self, void* , int a2));
CSF_FUNC(bool, __fastcall, AutoTask_CheckTarget, 0x0048C720, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, float angle));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(void, __fastcall, C_00833b88_RequestPrimaryAction, 0x00495270, (C_00833b88* self, void*));
CSF_FUNC(void, __fastcall, C_0083ab58_ContinueSearch, 0x004CD870, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083eb70_TestPairLink, 0x004E60B0, (C_0083eb70* self, void* , int a2));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

CSF_GLOBAL(float, g_SearchAngleScale, 0x0083AB30);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_0083ab58_UpdateSearchTarget(
    C_0083ab58* self, void* /*edx*/, int /*argument*/)
{
    // The packet also lists this address as a vtable reference.
    (void)CSF_VTBL(0x00492B20);

    alignas(8) uint8_t paramsBytes[0x24];
    alignas(8) uint8_t vectorBytes[0x14];

    auto* target = reinterpret_cast<void*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(&self->field_0044)));

    auto* linkedTask = reinterpret_cast<C_0083eb70*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 0x3C));

    if (!target
        || static_cast<uint8_t>(SceneObj_IsInactive(target, nullptr)) != 0
        || !linkedTask
        || CSF_AT(uint8_t, linkedTask, 0x4C) == 0
        || !C_0083eb70_TestPairLink(
            linkedTask, nullptr, reinterpret_cast<int>(&self->field_0044)))
    {
        C_00833b88_RequestPrimaryAction(
            reinterpret_cast<C_00833b88*>(self->field_0014), nullptr);
        CSF_AT(uint32_t, self, 0x48) = -1;
        self->field_004c = -1;
        self->field_0040 = 3;
        return;
    }

    void* owner = reinterpret_cast<void*>(self->field_0014);
    if (static_cast<uint8_t>(IsField1BCNonNegative(owner, nullptr)) != 0)
    {
        void* scene = g_pCurrentScene;
        char* handle = S_00492b20_GetPtr0x1B4(owner, nullptr);
        void* resolved = reinterpret_cast<void*>(
            CHandleTable_Resolve(scene, nullptr, reinterpret_cast<int>(handle)));

        if (resolved && CSF_AT(int, resolved, 0x50) == 19)
        {
            void* aimingObject = CSF_AT(void*, resolved, 0x4C);
            float* position =
                reinterpret_cast<float* (__fastcall*)(void*, void*, float*)>(
                    CSF_AT(void**, target, 0)[0xA4 / 4])(
                        target, nullptr, reinterpret_cast<float*>(vectorBytes));

            AiSearchOrders_AimLinkedObjectAtPosition(
                aimingObject, nullptr, position, 0);
        }
    }
    else
    {
        float* ownerPosition = GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0014), nullptr);
        float* targetPosition = GetObjectPositionVector(target, nullptr);
        float* facing = BuildFacingVectorBetween(
            targetPosition, nullptr,
            reinterpret_cast<float*>(vectorBytes), ownerPosition);
        double angle = S_00526d40_GetVectorAngleRadians(
            reinterpret_cast<S_00526d40*>(facing), nullptr);
        DispatchSceneObjectWithPayload(
            reinterpret_cast<void*>(self->field_0014), nullptr,
            static_cast<float>(angle * g_SearchAngleScale));
    }

    void* actionFsm = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0014), 0x44);
    int canAct = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, actionFsm, 0)[0xF4 / 4])(actionFsm, nullptr);

    if (!canAct)
    {
        C_0083ab58_ContinueSearch(self, nullptr);
        return;
    }

    AttackTasks_InitTaskBase(paramsBytes, nullptr, 1);
    bool targetMatches = AutoTask_CheckTarget(
        &self->field_0050, nullptr,
        reinterpret_cast<int>(
            reinterpret_cast<uint8_t*>(self->field_0014) + 0x1C),
        reinterpret_cast<int>(&self->field_0044));

    uint32_t searchValue = CSF_AT(uint32_t, self, 0x48);
    uint32_t searchLimit = self->field_004c;
    CSF_AT(uint32_t, paramsBytes, 0x0C) = searchValue;
    CSF_AT(uint32_t, paramsBytes, 0x10) = searchLimit;
    CSF_AT(uint8_t, paramsBytes, 0x20) = 1;
    CSF_AT(uint8_t, paramsBytes, 0x21) = 0;
    CSF_AT(uint8_t, paramsBytes, 0x22) = 1;
    CSF_AT(int, paramsBytes, 0x04) = static_cast<int>(targetMatches) + 1;
    CharActionFsm_CopyParamsFromSource(
        actionFsm, nullptr, reinterpret_cast<int>(paramsBytes));
}

HOOK(0x004CDA80, C_0083ab58_UpdateSearchTarget, "auto");
