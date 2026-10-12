#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiStateFactory_SubmitRequest28, 0x004B1D30, (void* a1, float a2, int a3, int a4));
CSF_FUNC(int, __fastcall, ActionArgs_ReadPair, 0x004B7040, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __stdcall AiStateFactory_HandleRequest28(C_0085e760* self)
{
    alignas(8) uint8_t firstArgumentBytes[0x4] = {};
    uint32_t secondArgument =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));

    const int linkedField = GetLinkedObjectField10(self, nullptr);
    ActionArgs_ReadPair(
        reinterpret_cast<void*>(static_cast<uintptr_t>(linkedField) + 100),
        nullptr, reinterpret_cast<uint32_t*>(firstArgumentBytes),
        &secondArgument);

    const int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<void*>(static_cast<uintptr_t>(linkedValue) - 4)
        : nullptr;

    // Unordered comparisons also proceed with the request.
    if (CSF_AT(float, linkedObject, 0x260) > g_fZero)
        return 0;

    GetLinkedObjectField10(self, nullptr);
    const int taskObject = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, taskObject + 44);

    const int requestSecondArgument = static_cast<int>(secondArgument);
    const int requestFirstArgument =
        static_cast<int>(CSF_AT(uint32_t, firstArgumentBytes, 0));

    const int entry = ResolveEntryLookupValue(self, nullptr);
    void* provider = GetCachedObjectProvider(
        reinterpret_cast<void*>(static_cast<uintptr_t>(entry)), nullptr);

    GetLinkedObjectField10(self, nullptr);
    AiStateFactory_SubmitRequest28(
        provider, 1.0f, requestFirstArgument, requestSecondArgument);
    return 1;
}

HOOK(0x004B7B00, AiStateFactory_HandleRequest28, "auto");
