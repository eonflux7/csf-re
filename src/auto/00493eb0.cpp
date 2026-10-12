#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, BuildQuerySegment, 0x0040FB60, (uint32_t* self, void* , const uint32_t* first, const uint32_t* second));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, CCharActionFsm_QueryCurrentAction, 0x00483510, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(bool, __fastcall, C_0083eb70_IsTargetInCone, 0x004E52A0, (C_0083eb70* self, void* , int targetHandle));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_008342e8_EvaluateInteractionTarget(
    C_008342e8* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x298) != 0 ||
        CSF_AT(int32_t, self, 0x248) < 0)
        return;

    auto* handle = reinterpret_cast<uint8_t*>(self) + 0x240;
    auto* target = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr,
                             reinterpret_cast<int>(handle)));
    if (!target)
        return;

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) ||
        !(target->field_0170 & 0x2000) ||
        target->field_0078 != 1 ||
        static_cast<uint8_t>(SceneObject_PassesFirstCheck(target, nullptr)) ||
        static_cast<uint8_t>(SceneObj_IsInactive(target, nullptr)) ||
        static_cast<uint8_t>(IsField1BCNonNegative(target, nullptr)))
        return;

    float interactionDistance = 0.0f;
    switch (CCharActionFsm_QueryCurrentAction(
        reinterpret_cast<void*>(self->field_0044), nullptr))
    {
    case 5:
        interactionDistance = CSF_AT(
            float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xB20);
        break;
    case 6:
        interactionDistance = CSF_AT(
            float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xB24);
        break;
    case 7:
        interactionDistance = CSF_AT(
            float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xB2C);
        break;
    case 8:
        interactionDistance = CSF_AT(
            float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xB28);
        break;
    default:
        break;
    }

    if (!(interactionDistance > CSF_AT(float, self, 0x258)))
        return;

    alignas(8) uint8_t segmentBytes[0x18];
    alignas(8) uint8_t startBytes[0xC];
    alignas(8) uint8_t endBytes[0xC];
    alignas(8) uint8_t queryFlagsBytes[0x4];

    void* position = GetObjectPositionVector(self, nullptr);
    float y = CSF_AT(float, position, 0x4);
    uint32_t x = CSF_AT(uint32_t, position, 0x0);
    uint32_t z = CSF_AT(uint32_t, position, 0x8);
    CSF_AT(uint32_t, startBytes, 0x0) = x;
    CSF_AT(uint32_t, startBytes, 0x8) = z;
    CSF_AT(float, startBytes, 0x4) = y + g_fReturnedConstant_0082E030;

    position = GetObjectPositionVector(target, nullptr);
    y = CSF_AT(float, position, 0x4);
    x = CSF_AT(uint32_t, position, 0x0);
    z = CSF_AT(uint32_t, position, 0x8);
    CSF_AT(uint32_t, endBytes, 0x0) = x;
    CSF_AT(float, endBytes, 0x4) = y + g_fReturnedConstant_0082E030;
    CSF_AT(uint32_t, endBytes, 0x8) = z;

    BuildQuerySegment(reinterpret_cast<uint32_t*>(segmentBytes), nullptr,
                      reinterpret_cast<const uint32_t*>(startBytes),
                      reinterpret_cast<const uint32_t*>(endBytes));

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(sceneLoader, nullptr));
    void* query = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[0x3C / 4])(controller, nullptr);

    CSF_AT(uint32_t, queryFlagsBytes, 0) = 16;
    if (reinterpret_cast<bool (__fastcall*)(void*, void*, uint8_t*, uint32_t*, int)>(
            CSF_AT(void**, query, 0)[0x24 / 4])(
            query, nullptr, segmentBytes,
            reinterpret_cast<uint32_t*>(queryFlagsBytes), 3))
        return;

    void* targetCone = reinterpret_cast<void*>(target->field_003c);
    if (CSF_AT(uint8_t, targetCone, 0x4C) &&
        CSF_AT(uint32_t, targetCone, 0x30) == 1 &&
        C_0083eb70_IsTargetInCone(
            reinterpret_cast<C_0083eb70*>(targetCone), nullptr,
            reinterpret_cast<int>(&self->field_001c)))
        return;

    CSF_AT(uint32_t, self, 0x298) = 2;
    CSF_AT(uint32_t, self, 0x2A0) = CSF_AT(uint32_t, handle, 0x4);
    CSF_AT(uint32_t, self, 0x2A4) = CSF_AT(uint32_t, handle, 0x8);
    CSF_AT(int, self, 0x2AC) =
        MenuExtras_ParseDescriptionId(CSF_STR(0x00833DA8, "G001"));
}

HOOK(0x00493EB0, C_008342e8_EvaluateInteractionTarget, "auto");
