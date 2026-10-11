#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void, __stdcall, AiStateFactory_InitializeDistanceTask, 0x004C0510, (void* a1, float a2, float a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);

static char __stdcall C_0085e760_TryAdvanceDistanceTask(C_0085e760* self)
{
    void* linkedState = reinterpret_cast<void*>(GetLinkedObjectField10(self, nullptr));
    const float distance = CSF_AT(float, CSF_AT(void*, linkedState, 0x74), 0);
    const std::uint32_t remaining = CSF_AT(std::uint32_t, linkedState, 0x7C) - 1;

    CSF_AT(std::uint32_t, linkedState, 0x7C) = remaining;
    if (remaining != 0)
    {
        void* entries = CSF_AT(void*, linkedState, 0x74);
        _memcpy(entries, static_cast<std::uint8_t*>(entries) + 4, 4 * remaining);
    }

    if (distance > g_fZero)
    {
        int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        void* object = linkedValue
            ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
            : nullptr;

        if (CSF_AT(int, object, 0x608) == -1 &&
            CSF_AT(int, object, 0x60C) == -1)
        {
            return 0;
        }

        linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        void* positionOwner = linkedValue
            ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
            : nullptr;

        linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        void* destinationOwner = linkedValue
            ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
            : nullptr;

        float* position = GetObjectPositionVector(
            CSF_AT(void*, positionOwner, 0x2C), nullptr);

        const float dx = CSF_AT(float, destinationOwner, 0x64C) - position[0];
        const float dy = CSF_AT(float, destinationOwner, 0x650) - position[1];
        const float dz = CSF_AT(float, destinationOwner, 0x654) - position[2];
        const float radius = distance + g_Const200_0f;

        if (!(radius * radius < dz * dz + dx * dx + dy * dy))
            return 0;
    }

    GetLinkedObjectField10(self, nullptr);
    const int taskState = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, taskState + 0x24);

    const int entry = ResolveEntryLookupValue(self, nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(entry), nullptr);
    GetLinkedObjectField10(self, nullptr);
    AiStateFactory_InitializeDistanceTask(provider, 0.0f, distance);
    return 1;
}

HOOK(0x004C3CF0, C_0085e760_TryAdvanceDistanceTask, "auto");
