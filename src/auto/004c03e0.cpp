#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(const float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ActionArgs_ReadPair, 0x004B7040, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, AiCore_PrepareCoverAttack, 0x004BCBA0, (void* self, void* , void* linkedObject, const std::uint32_t* targetPosition, int attackType, int attackFlags));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static bool __stdcall C_0085e760_TryPrepareDistantCoverAttack(C_0085e760* self)
{
    alignas(8) uint8_t attackTypeBytes[0x4];
    uint32_t& attackType = *reinterpret_cast<uint32_t*>(attackTypeBytes);
    attackType = 0;
    uint32_t rangeBits = reinterpret_cast<uintptr_t>(self);

    auto* linkedState =
        reinterpret_cast<uint8_t*>(GetLinkedObjectField10(self, nullptr));
    ActionArgs_ReadPair(linkedState + 0x64, nullptr, &attackType, &rangeBits);

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    auto* linkedBase = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    if (CSF_AT(int, linkedBase, 0x608) == -1 &&
        CSF_AT(int, linkedBase, 0x60C) == -1)
        return false;

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    auto* positionOwner = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    auto* targetOwner = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    const float* position = GetObjectPositionVector(
        CSF_AT(void*, positionOwner, 0x2C), nullptr);

    const float deltaX = CSF_AT(float, targetOwner, 0x64C) - position[0];
    const float deltaY = CSF_AT(float, targetOwner, 0x650) - position[1];
    const float deltaZ = CSF_AT(float, targetOwner, 0x654) - position[2];

    float range;
    std::memcpy(&range, &rangeBits, sizeof(range));
    const float distanceSquared =
        deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX;

    if (!(range * range < distanceSquared))
        return false;

    GetLinkedObjectField10(self, nullptr);
    const int taskState = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, taskState + 0x34);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    auto* targetBase = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    const int attackFlags = static_cast<int>(rangeBits);
    const int selectedAttackType = static_cast<int>(attackType);
    const auto* targetPosition =
        reinterpret_cast<const std::uint32_t*>(targetBase + 0x64C);

    void* entry = reinterpret_cast<void*>(ResolveEntryLookupValue(self, nullptr));
    void* provider = GetCachedObjectProvider(entry, nullptr);

    linkedState =
        reinterpret_cast<uint8_t*>(GetLinkedObjectField10(self, nullptr));
    AiCore_PrepareCoverAttack(
        linkedState + 0x34, nullptr, provider, targetPosition,
        selectedAttackType, attackFlags);
    return true;
}

HOOK(0x004C03E0, C_0085e760_TryPrepareDistantCoverAttack, "auto");
