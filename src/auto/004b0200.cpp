#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiCore_IsTargetPositionValid, 0x004AEDD0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(char, __fastcall, AiFncStatic_CheckPositionPair, 0x004DC660, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);

static bool __fastcall S_004b0200_CanSeeTargetPosition(
    S_004b0200* self, void* /*edx*/, int character, int targetPosition)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(character), nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    if (static_cast<uint8_t>(AiCore_IsTargetPositionValid(
            linkedObject, nullptr, targetPosition)) == 0)
        return false;

    linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(character), nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    const uint32_t characterData = CSF_AT(uint32_t, linkedObject, 0x2C);
    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* positionChecker = reinterpret_cast<void*>(
        CSF_AT(uint32_t, manager, 0x1FD8));

    if (!AiFncStatic_CheckPositionPair(
            static_cast<uint8_t*>(positionChecker) + 0x54, nullptr,
            static_cast<int>(characterData + 0x1C), targetPosition))
        return false;

    alignas(8) uint8_t segmentBytes[0x30];
    void* target = reinterpret_cast<void*>(targetPosition);

    const float targetHeight =
        CSF_AT(float, target, 0x04) + g_fReturnedConstant_0082E030;
    const uint32_t targetZ = CSF_AT(uint32_t, target, 0x08);
    const uint32_t originX = self->field_0004;
    const uint32_t targetX = CSF_AT(uint32_t, target, 0x00);
    const float originHeight =
        self->field_0008 + g_fReturnedConstant_0082E030;
    const uint32_t originZ = self->field_000c;

    CSF_AT(uint32_t, segmentBytes, 0x18) = targetX;
    CSF_AT(float, segmentBytes, 0x1C) = targetHeight;
    CSF_AT(uint32_t, segmentBytes, 0x20) = targetZ;
    CSF_AT(uint32_t, segmentBytes, 0x24) = originX;
    CSF_AT(float, segmentBytes, 0x28) = originHeight;
    CSF_AT(uint32_t, segmentBytes, 0x2C) = originZ;

    CSF_AT(uint32_t, segmentBytes, 0x00) = originX;
    CSF_AT(float, segmentBytes, 0x04) = originHeight;
    CSF_AT(uint32_t, segmentBytes, 0x08) = originZ;
    CSF_AT(uint32_t, segmentBytes, 0x0C) = targetX;
    CSF_AT(float, segmentBytes, 0x10) = targetHeight;
    CSF_AT(uint32_t, segmentBytes, 0x14) = targetZ;

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(globalObject, nullptr));

    void* collisionQuery = reinterpret_cast<void*>(
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, sceneController, 0)[0x3C / 4])(
                sceneController, nullptr));

    return reinterpret_cast<int(__fastcall*)(void*, void*, int, void*, int)>(
        CSF_AT(void**, collisionQuery, 0)[0x38 / 4])(
            collisionQuery, nullptr, 0, segmentBytes, 3) == 0;
}

HOOK(0x004B0200, S_004b0200_CanSeeTargetPosition, "auto");
