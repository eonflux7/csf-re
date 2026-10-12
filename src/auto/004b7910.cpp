#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082f9f8_Start, 0x004226C0, (C_0082f9f8* self, void* , float a2, char a3));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_DegreesToRadians, 0x00835440);

static char __stdcall AiStateFactory_HandleRotationRequest25(C_0085e760* self)
{
    void* linkedObject = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(GetLinkedObjectField10(self, nullptr)));

    const float queuedAngle =
        CSF_AT(float, CSF_AT(void*, linkedObject, 0x74), 0);
    const std::uint32_t count = CSF_AT(std::uint32_t, linkedObject, 0x7C);
    auto* queue = static_cast<std::uint8_t*>(linkedObject) + 100;
    const std::uint32_t remainingCount = count - 1;

    if (remainingCount != 0)
    {
        CSF_AT(std::uint32_t, queue, 24) = remainingCount;
        void* entries = CSF_AT(void*, queue, 16);
        _memcpy(entries, static_cast<std::uint8_t*>(entries) + 4,
                4 * remainingCount);
    }
    else
    {
        CSF_AT(std::uint32_t, queue, 24) = 0;
    }

    linkedObject = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(GetLinkedObjectField10(self, nullptr)));
    AiCoverAttackTasks_ResetLinkedTaskState(
        self, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(linkedObject) + 40));

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    void* linkedOwner = reinterpret_cast<void*>(
        linkedValue != 0
            ? static_cast<std::uint32_t>(linkedValue) - 4
            : 0);

    const float angle = static_cast<float>(
        C_008342e8_ComputeResolvedObjectScalar(
            CSF_AT(C_008342e8*, linkedOwner, 44), nullptr)
        * g_DegreesToRadians + queuedAngle);

    void* entry = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(ResolveEntryLookupValue(self, nullptr)));
    void* provider = GetCachedObjectProvider(entry, nullptr);

    GetLinkedObjectField10(self, nullptr);
    linkedValue = GetLinkedObjectValue_4DB5C0(provider, nullptr);
    linkedOwner = reinterpret_cast<void*>(
        linkedValue != 0
            ? static_cast<std::uint32_t>(linkedValue) - 4
            : 0);

    void* resolvedObject = CSF_AT(void*, linkedOwner, 44);
    C_0082f9f8_Start(
        CSF_AT(C_0082f9f8*, resolvedObject, 48), nullptr, angle, 1);
    return 1;
}

HOOK(0x004B7910, AiStateFactory_HandleRotationRequest25, "auto");
