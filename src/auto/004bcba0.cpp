#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void, __stdcall, AiCore_AdjustPositionToSceneSurface, 0x004AA6D0, (void* a1));
CSF_FUNC(unsigned int, __fastcall, CoverAttack_Sub_4B9BA0, 0x004B9BA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));

[[maybe_unused]] static const auto g_TaskTargetNameVtable = CSF_VTBL(0x0047AE10);

static int __fastcall AiCore_PrepareCoverAttack(
    void* self, void* /*edx*/, void* linkedObject,
    const std::uint32_t* targetPosition, int attackType, int attackFlags)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    auto* actor = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;
    CoverAttack_Sub_4B9BA0(actor, nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    actor = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    auto* position = reinterpret_cast<const std::uint32_t*>(
        GetObjectPositionVector(CSF_AT(void*, actor, 0x2C), nullptr));

    CSF_AT(std::uint32_t, self, 0x04) = position[0];
    CSF_AT(std::uint32_t, self, 0x08) = position[1];
    CSF_AT(std::uint32_t, self, 0x0C) = position[2];
    CSF_AT(std::uint32_t, self, 0x10) = targetPosition[0];
    CSF_AT(std::uint32_t, self, 0x14) = targetPosition[1];
    CSF_AT(std::uint32_t, self, 0x18) = targetPosition[2];
    CSF_AT(int, self, 0x20) = attackFlags;
    CSF_AT(int, self, 0x1C) = attackType;

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    actor = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    void* task = CSF_AT(void*, actor, 0x718);
    int targetData = GetLinkedObjectField10(linkedObject, nullptr) + 148;
    char* targetNameBuffer = AiCore_GetTaskTargetNameBuffer(task, nullptr);
    AiCore_UpdateTargetData(targetNameBuffer, nullptr, targetData);

    alignas(8) std::uint8_t adjustedPositionBytes[0xC];
    auto& adjustedPosition =
        *reinterpret_cast<S_004e0c20*>(adjustedPositionBytes);

    const std::uint32_t targetZ = targetPosition[2];
    const std::uint32_t targetX = targetPosition[0];
    const std::uint32_t targetY = targetPosition[1];
    std::memcpy(&adjustedPosition.field_0008, &targetZ, sizeof(targetZ));
    CSF_AT(std::uint32_t, &adjustedPosition, 0x00) = targetX;
    CSF_AT(std::uint32_t, &adjustedPosition, 0x04) = targetY;

    GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    AiCore_AdjustPositionToSceneSurface(&adjustedPosition);

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    actor = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    void* worldObject = CSF_AT(void*, actor, 0x2C);
    void* controller = CSF_AT(void*, worldObject, 0x30);
    return reinterpret_cast<int(__fastcall*)(void*, void*, S_004e0c20*, int)>(
        CSF_AT(void**, controller, 0)[0xAC / 4])(
            controller, nullptr, &adjustedPosition, 0);
}

HOOK(0x004BCBA0, AiCore_PrepareCoverAttack, "auto");
